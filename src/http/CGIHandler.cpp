/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 10:49:55 by rmhazres          #+#    #+#             */
/*   Updated: 2026/06/08 13:27:19 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./CGiHandler.hpp"
#include "../common/Utils.hpp"

#include <cstddef>
#include <map>
#include <string>
#include <vector>

void CGIHanlder::execute(const HttpRequest& request, const Server& server, HttpResponse& response)
{
	const LocationBlock* block = findMatchingLocation(request.getTarget(), server);

	std::string scriptPath = ((bool)block && block->getRoot().has_value() ? block->getRoot().value() : server.getRoot() + request.getTarget());
	
	
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
	// redirect status ? 
	// all other envs ?
	return env;
};

void CGIHanlder::executeCGI(const std::string& scriptPath, const std::vector<std::string>& env, const std::string& body, HttpResponse& response)
{
	
}