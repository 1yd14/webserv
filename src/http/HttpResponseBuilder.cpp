/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponseBuilder.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 16:15:48 by rmhazres          #+#    #+#             */
/*   Updated: 2026/06/04 14:18:53 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponseBuilder.hpp"
#include <array>
#include <cstddef>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <map>
#include <string>
#include <bits/stdc++.h>
#include <cstdio>
#include <vector>

HttpResponseBuilder::HttpResponseBuilder() = default;
HttpResponseBuilder::~HttpResponseBuilder(){};

HttpResponse HttpResponseBuilder::build(HttpRequest const &request, Server const &server, RouteType routeType )
{
	HttpResponse response;

	buildProtocol(request,response);
	buildStatusCode(request, response,server, routeType);
	buildBody(request, response, server, routeType);
	buildHeader(request, response);

	return response;
};

void HttpResponseBuilder::buildProtocol(const HttpRequest& request,HttpResponse& response)
{
	response.setProtocol(request.getProtocol());
};
void HttpResponseBuilder::buildStatusCode(const HttpRequest& request, HttpResponse& response, const Server& server, RouteType routeType)
{
	if (routeType == RouteType::NOT_FOUND)
	{
		response.setStatus(HttpStatus::NOT_FOUND);
		return;
	}
	
	if (routeType == RouteType::STATIC_FILE)
	{
		if(!std::filesystem::exists(server.getRoot() + request.getTarget()))
		{
			response.setStatus(HttpStatus::NOT_FOUND);	
		}
		else {
			response.setStatus(HttpStatus::OK);
		}
		return;
	}
	
	if (routeType == RouteType::REDIRECT)
	{
		for (const auto& location : server.getLocationBlocks())
			{
				if (request.getTarget().find(location.getPath()) == 0)
				{
					response.setStatus((HttpStatus)location.getRedirectCode().value());
					return;
				}
			}
	}
	if (routeType == RouteType::DELETE_FILE)
	{
		response.setStatus(HttpStatus::NO_CONTENT);
		return ;
	}
	if (routeType == RouteType::CGI)
	{
		response.setStatus(HttpStatus::OK);
		return;
	}
	if (routeType == RouteType::UPLOAD)
	{
		response.setStatus(HttpStatus::CREATED);
		return;
	}
	if (routeType == RouteType::DIRECTORY_LISTING) 
	{
		response.setStatus(HttpStatus::OK);
		return;
	}
}
void HttpResponseBuilder::buildHeader(const HttpRequest& request, HttpResponse& response)
{
	/*Content-Type — what type of content is being sent
		Content-Length — size of the body
		Date — current date/time
		Server — server name (e.g. webserv/1.0)
		Connection — keep-alive or close*/
		
		const std::map<std::string, std::string> headers = request.getHeader();
		auto itt = headers.find("content-type");
		if (itt != headers.end())
		{
			response.setHeader(itt->first, itt->second);
		}
	
	response.setHeader("Content-Length", std::to_string(response.getBody().length()));
		
	std::time_t time = std::time(nullptr);
	std::array<char, 100> mbstr;
	std::strftime(mbstr.data(), sizeof(mbstr), "%a, %d %b %Y %H:%M:%S GMT", std::gmtime(&time));
	response.setHeader("Date", std::string(mbstr.data()));
	response.setHeader("Server", "WebServ");

	auto itt1 = headers.find("connection");
	if (itt1 != headers.end())
	{
		response.setHeader(itt1->first, itt1->second);
	}
	else
	{
		if (request.getProtocol() == "HTTP/1.1")
		{
			response.setHeader("Connection", "keep-alive");
		}
		else 
		{
			response.setHeader("Connection", "close");
		}
	}
}

void HttpResponseBuilder::buildBody(const HttpRequest& request,HttpResponse& response,const Server& server,RouteType routeType)
{
	const std::string path = server.getRoot() + request.getTarget();
	const std::string uploadDir = server.getLocationBlocks().
	if (routeType == RouteType::STATIC_FILE)
	{
		manageStatic(response, path);
		return;
	}
	if (routeType == RouteType::DELETE_FILE)
	{
		manageDelete(response, path);
		return;
	}
	if (routeType == RouteType::UPLOAD)
	{
		manageUpload(response,path, request, server);
		return;
	}
	// switch (routeType) {
	// 	case RouteType::STATIC_FILE:
	// 		manageStatic(response, path);
	// 		break;
	// 	case RouteType::DELETE_FILE:
	// 		manageDelete(response,path);
	// 		break;
	// 	case RouteType::UPLOAD:
	// 		manageUpload(response, path);
	// 		break;
	// }

}

void HttpResponseBuilder::manageStatic(HttpResponse& response,const std::string path)
{
		std::ifstream file(path);
		if(!file.is_open())
		{
			response.setStatus(HttpStatus::NOT_FOUND);
			return ;
		}
		std::string body((std::istreambuf_iterator<char>(file)),
						  std::istreambuf_iterator<char>());
		response.setBody(body);
}
void HttpResponseBuilder::manageDelete(HttpResponse& response, const std::string path)
{
	
	int status = std::remove(path.c_str());
	if (status != 0)
	{
		response.setStatus(HttpStatus::NOT_FOUND);
		return ;
	}
	response.setStatus(HttpStatus::NO_CONTENT);
}

void HttpResponseBuilder::manageUpload(HttpResponse& response, std::string path , const HttpRequest& request, const Server& server)
{
	std::vector<LocationBlock> blocks = server.getLocationBlocks();
	for (const auto& location : blocks)
	{
		if (request.getTarget().find(location.getPath()) == 0)
		{	
		}
	}
}