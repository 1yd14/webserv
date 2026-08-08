/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 10:49:55 by rmhazres          #+#    #+#             */
/*   Updated: 2026/08/08 15:18:38 by lyvan-de         ###   ########.fr       */
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
#include <iostream>

std::vector<std::string> CGIHanlder::buildEnv(const HttpRequest& request, const Server& server, int localPort)
{
	std::vector<std::string> env;
	std::string path = request.getTarget();
	size_t qmark =path.find("?");
	const LocationBlock* block = findMatchingLocation(path, server);
	std::string scriptPath = getScriptPath(request, server, *block);
	std::string scriptName = (qmark != std::string::npos) ? path.substr(0, qmark) : path;
    std::string query      = (qmark != std::string::npos) ? path.substr(qmark + 1) : "";

	env.emplace_back("GATEWAY_INTERFACE=CGI/1.1");
    env.emplace_back("SERVER_SOFTWARE=webserv/1.0");
	env.emplace_back("SERVER_PROTOCOL=" + request.getProtocol());
	env.emplace_back("SERVER_NAME=" + server.getHost());
	env.emplace_back("SERVER_PORT=" + std::to_string(localPort));
	env.emplace_back("REQUEST_METHOD=" + request.getMethod());
	env.emplace_back("SCRIPT_NAME=" + scriptName);
	env.emplace_back("SCRIPT_FILENAME=" + scriptPath);
	env.emplace_back("PATH_INFO=");
	env.emplace_back("QUERY_STRING=" + query);
	env.emplace_back("CONTENT_LENGTH=" + std::to_string(request.getContentLength()));
	
	std::map<std::string, std::string>  header = request.getHeader();
	auto itt = header.find("content-type");
	env.emplace_back("CONTENT_TYPE="+ (itt != header.end() ? itt->second : ""));
	env.emplace_back("REDIRECT_STATUS=200");
	// redirect status ? 
	// all other envs ?
	return env;
};

std::string CGIHanlder::getScriptPath(const HttpRequest& request, const Server& server, const LocationBlock& block)
{
	std::string scriptPath;
	if (block.getRoot().has_value())
	{
		std::string target = request.getTarget();
		std::string locationPath = block.getPath();
		std::string relative = target.substr(locationPath.size());
		scriptPath = block.getRoot().value() + relative;
		
	}else {
		scriptPath =  server.getRoot() + request.getTarget();
	}
	return scriptPath;
}

std::string CGIHanlder::getInterpreter(std::string extension)
{
	if (extension == ".py")
	{
		return "/usr/bin/python3" ;
	} 
	if (extension == ".php") 
	{
		return "/usr/bin/php-cgi";
	}

	return "";
}


std::vector<std::string> CGIHanlder::buildArgs(const HttpRequest& request, const Server& server)
{
	std::vector<std::string> argv;
	const LocationBlock* block = findMatchingLocation(request.getTarget(), server);
	if (block == nullptr)
	{
		return argv;
	}
	std::string scriptPath = getScriptPath(request, server, *block);
	size_t qpos = scriptPath.find('?');
	if (qpos != std::string::npos)
	{
    	scriptPath = scriptPath.substr(0, qpos);
	}
	std::string extension;
	if (block->getCgiExtension().has_value())
	{
		extension = block->getCgiExtension().value();
	}
	std::string interpreter = getInterpreter(extension);
	argv.push_back(interpreter);
	if (extension == ".py")
	{
		argv.push_back(scriptPath);
	}
	return  argv;
}

HttpResponse CGIHanlder::buildError(const HttpStatus& status, const Server& server)
{
	HttpResponse errorResponse;
	
	errorResponse.setProtocol("HTTP/1.1");
	errorResponse.setStatus(status);
	getErrorBody(errorResponse, server);
	errorResponse.setHeader("Content-Length", std::to_string(errorResponse.getBody().length()));
	return errorResponse;
}

void CGIHanlder::execute(const HttpRequest& request,const Server& server, EventLoop& loop, Connection& connection)
{
	std::vector<std::string> env = buildEnv(request, server, connection.getLocalPort());
	std::vector<std::string> argv = buildArgs(request, server);
	std::string errorResponse = buildError(HttpStatus::INTERNAL_SERVER_ERROR, server).serialize();
	if (argv.empty())
	{
		connection.setWriterBuffer(errorResponse);
		connection.setState(WRITING);
		loop.setWriting(&connection, EPOLL_CTL_MOD);
		return;
	}
	const LocationBlock* block = findMatchingLocation(request.getTarget(), server);
	if (block == nullptr)
	{
		errorResponse = buildError(HttpStatus::NOT_FOUND, server).serialize();
		connection.setWriterBuffer(errorResponse);
		connection.setState(WRITING);
		loop.setWriting(&connection, EPOLL_CTL_MOD);
		return;
	}
	std::string scriptPath = getScriptPath(request, server, *block);
	size_t qpos = scriptPath.find('?');
	if (qpos != std::string::npos)
	{
		scriptPath = scriptPath.substr(0, qpos);
	}
	std::array<int, 2> pipe_in;
	std::array<int, 2> pipe_out;

	if (access(scriptPath.c_str(), X_OK) != 0)
	{
   		errorResponse = buildError(HttpStatus::FORBIDDEN, server).serialize();
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
		
		loop.addCgi(std::make_unique<CGIProcess>(pipe_out[0], pid, connection.getId(), server));
	}
}

void CGIHanlder::parseCGIOutput(const std::string& output, HttpResponse& response, const Server& server)
{
	size_t separator = output.find("\r\n\r\n");
	
	response.setProtocol("HTTP/1.1");

	if (separator == std::string::npos)
	{
		response.setStatus(HttpStatus::INTERNAL_SERVER_ERROR);
		getErrorBody(response, server);
		response.setHeader("Content-Length", std::to_string(response.getBody().length()));
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