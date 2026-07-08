/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 10:49:55 by rmhazres          #+#    #+#             */
/*   Updated: 2026/07/08 15:58:23 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./CGiHandler.hpp"
#include "../common/Utils.hpp"
#include "CGIProcess.hpp"

#include <array>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>


std::vector<std::string> CGIHanlder::buildEnv(const HttpRequest& request, const Server& server)
{
	std::vector<std::string> env;
	std::string path = request.getTarget();
	size_t qmark =path.find("?");

	env.emplace_back("REQUEST_METHOD="+ request.getMethod());
	
	env.emplace_back("PATH_INFO="+ (qmark != std::string::npos ? path.substr(0,qmark) : path));
	env.emplace_back("QUERY_STRING="+ (qmark != std::string::npos ? path.substr(qmark+1) : ""));
	env.emplace_back("CONTENT_LENGTH="+ std::to_string(request.getContentLength()));
	
	std::map<std::string, std::string>  header = request.getHeader();
	auto itt = header.find("content-type");
	env.emplace_back("CONTENT_TYPE="+ (itt != header.end() ? itt->second : ""));
	env.emplace_back("SCRIPT_FILENAME=" + server.getRoot() + path);
	env.emplace_back("SERVER_PROTOCOL=" + request.getProtocol());
	env.emplace_back("SERVER_NAME="+server.getHost());
	env.emplace_back("SERVER_PORT="+ std::to_string(server.getPort()));
	env.emplace_back("REDIRECT_STATUS=200");
	// redirect status ? 
	// all other envs ?
	return env;
};


std::vector<std::string> CGIHanlder::buildArgs(const HttpRequest& request, const Server& server)
{
	std::string scriptPath;
	std::string extention;
	std::string interpreter;
	std::vector<std::string> argv;
	const LocationBlock* block = findMatchingLocation(request.getTarget(),server);
	if (block == nullptr)
	{
		return argv;
	}
	
	if (block->getRoot().has_value())
	{
		std::string target = request.getTarget();
		std::string locationPath = block->getPath();
		std::string relative = target.substr(locationPath.size());
		scriptPath = block->getRoot().value() + relative;
		
	}else {
		scriptPath =  server.getRoot() + request.getTarget();
	}
	if (block->getCgiExtension().has_value())
	{
		extention = block->getCgiExtension().value();
	}
	if (extention == ".py")
	{
		interpreter = "/usr/bin/python3";
	} else if (extention == ".php") 
	{
		interpreter = "/usr/bin/php-cgi";
	}
	else 
	{
		return argv; 
	}
	argv.push_back(interpreter);
	argv.push_back(scriptPath);
	return  argv;
}

HttpResponse CGIHanlder::buildError(const HttpStatus& status)
{
	HttpResponse errorResponse;
	
	errorResponse.setStatus(status);
	errorResponse.setProtocol("HTTP/1.1");
	errorResponse.setHeader("Content-Length", "0");
	return errorResponse;
}

void CGIHanlder::execute(const HttpRequest& request,const Server& server, EventLoop& loop, Connection& connection)
{
	std::vector<std::string> env = buildEnv(request, server);
	std::vector<std::string> argv = buildArgs(request, server);
	std::string errorResponse = buildError(HttpStatus::INTERNAL_SERVER_ERROR).serialize();

	
	if (argv.empty())
	{
		connection.setWriterBuffer(errorResponse);
		connection.setState(WRITING);
		loop.setWriting(&connection, EPOLL_CTL_MOD);
		return;
	}
	std::array<int, 2> pipe_in;
	std::array<int, 2> pipe_out;

	if (access(argv[1].c_str(), X_OK) != 0)
	{
   		errorResponse = buildError(HttpStatus::FORBIDDEN).serialize();
		connection.setWriterBuffer(errorResponse);
		connection.setState(WRITING);
		loop.setWriting(&connection, EPOLL_CTL_MOD);

		return;
	}
	if (pipe(pipe_in.data()) < 0)
	{

		connection.setWriterBuffer(errorResponse);
		connection.setState(WRITING);
		connection.setState(WRITING);
		loop.setWriting(&connection, EPOLL_CTL_MOD);

		return;
	}
	if (pipe(pipe_out.data()) < 0)
	{
		connection.setWriterBuffer(errorResponse);
		connection.setState(WRITING);
		loop.setWriting(&connection, EPOLL_CTL_MOD);
		close(pipe_in[0]);
		close(pipe_in[1]);
		return;
	}

	pid_t pid = fork();
	if (pid < 0)
	{
		connection.setWriterBuffer(errorResponse);
		connection.setState(WRITING);
		loop.setWriting(&connection, EPOLL_CTL_MOD);

		close(pipe_in[0]);
		close(pipe_in[1]);
		close(pipe_out[0]);
		close(pipe_out[1]);
		return;
	}
	if (pid == 0)
	{
		close(pipe_in[1]);
		close(pipe_out[0]);
		dup2(pipe_in[0], STDIN_FILENO);
		close(pipe_in[0]);
		dup2(pipe_out[1], STDOUT_FILENO);
		close(pipe_out[1]);
		
		std::vector<char *> envp;
		for (const auto& ett:env)
		{
			envp.push_back(const_cast<char*>(ett.c_str()));
		}
		envp.push_back(nullptr);

		std::vector<char *> argvp;
		for (const auto& ett:argv)
		{
			argvp.push_back(const_cast<char*>(ett.c_str()));
		}
		argvp.push_back(nullptr);
		
		
		execve(argvp[0], argvp.data(), envp.data());
		exit(1);
	}
	else 
	{
		close(pipe_in[0]);
		close(pipe_out[1]);
		
		write(pipe_in[1],request.getBody().c_str(),request.getBody().length());
		close(pipe_in[1]);
		
		loop.addCgi(std::make_unique<CGIProcess>(pipe_out[0], pid, connection));
	}
}

void CGIHanlder::parseCGIOutput(const std::string& output, HttpResponse& response)
{
	size_t separator = output.find("\r\n\r\n");
	
	response.setProtocol("HTTP/1.1");

	if (separator == std::string::npos)
	{
		response.setStatus(HttpStatus::INTERNAL_SERVER_ERROR);
		return;
	}

	response.setHeader(parseHeaders(output.substr(0,separator)));
	response.setBody(output.substr(separator+4));
	response.setStatus(HttpStatus::OK);
	response.setHeader("content-length", std::to_string(response.getBody().length()));
	const auto& header = response.getHeader();
	auto itt = header.find("status");
	if (itt != header.end())
	{
		response.setStatus((HttpStatus)safeConvertLong(itt->second));
	}
}