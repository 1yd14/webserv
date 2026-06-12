/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 10:49:55 by rmhazres          #+#    #+#             */
/*   Updated: 2026/06/12 16:47:26 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./CGiHandler.hpp"
#include "../common/Utils.hpp"

#include <array>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>

void CGIHanlder::execute(const HttpRequest& request, const Server& server, HttpResponse& response, const LocationBlock& block)
{
	std::string scriptPath;
	std::string extention;
	std::string interpreter;

	
	if (block.getRoot().has_value())
	{
		std::string target = request.getTarget();
		std::string locationPath = block.getPath();
		std::string relative = target.substr(locationPath.size());
		scriptPath = block.getRoot().value() + relative;
		
	}else {
		scriptPath =  server.getRoot() + request.getTarget();
	}
	if (block.getCgiExtension().has_value())
	{
		extention = block.getCgiExtension().value();
	}
	if (extention == ".py")
	{
		interpreter = "/usr/bin/python3";
	} else if (extention == ".php") 
	{
		interpreter = "/usr/bin/php-cgi";
	}
	else {
		response.setStatus(HttpStatus::NOT_IMPLEMENTED);
		return;
	}
	std::vector<std::string> env = buildEnv(request, server);
	executeCGI(interpreter,scriptPath, env, response, request.getBody());
}

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

void CGIHanlder::executeCGI(const std::string& interpreter , const std::string& scriptPath,const std::vector<std::string>& env, HttpResponse& response, const std::string& body)
{
	std::array<int, 2> pipe_in;
	std::array<int, 2> pipe_out;
	
	std::vector<char*> argv;
	argv.push_back(const_cast<char*>(interpreter.c_str()));
	argv.push_back(const_cast<char*>(scriptPath.c_str()));
	argv.push_back(nullptr);
	int status;
	
	if (pipe(pipe_in.data()) < 0)
	{
		response.setStatus(HttpStatus::INTERNAL_SERVER_ERROR);
		return;
	}
	if (pipe(pipe_out.data()) < 0)
	{
		close(pipe_in[0]);
		close(pipe_in[1]);
		response.setStatus(HttpStatus::INTERNAL_SERVER_ERROR);
		return;
	}

	pid_t pid = fork();
	if (pid < 0)
	{
		close(pipe_in[0]);
		close(pipe_in[1]);
		close(pipe_out[0]);
		close(pipe_out[1]);
		response.setStatus(HttpStatus::INTERNAL_SERVER_ERROR);
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
		
		execve(argv[0], const_cast<char* const*>(argv.data()), envp.data());
		exit(1);
	}
	else 
	{
		close(pipe_in[0]);
		close(pipe_out[1]);
		
		write(pipe_in[1],body.c_str(),body.length());
		close(pipe_in[1]);
		
		std::string output;
		std::array<char, 4096> buffer;
		ssize_t bytes;
		while((bytes = read(pipe_out[0], buffer.data(), sizeof(buffer))) > 0)
		{
			output.append(buffer.data(), bytes);
		}
		parseCGIOutput(output, response);
		close(pipe_out[0]);
		waitpid(pid, &status, 0);
	}
}

void CGIHanlder::parseCGIOutput(const std::string& output, HttpResponse& response)
{
	size_t separator = output.find("\r\n\r\n");
	
	if (separator == std::string::npos)
	{
		response.setStatus(HttpStatus::INTERNAL_SERVER_ERROR);
		return;
	}
	std::cout << " oputput " << output << "\n";
	response.setHeader(parseHeaders(output.substr(0,separator)));
	response.setBody(output.substr(separator+4));
	
	const auto& header = response.getHeader();
	auto itt = header.find("status");
	if (itt != header.end())
	{
		response.setStatus((HttpStatus)safeConvertLong(itt->second));
	}
}