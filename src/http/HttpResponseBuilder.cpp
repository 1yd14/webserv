/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponseBuilder.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 16:15:48 by rmhazres          #+#    #+#             */
/*   Updated: 2026/07/13 10:21:37 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponseBuilder.hpp"
#include "../common/Utils.hpp"
#include <array>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <bits/stdc++.h>
#include <cstdio>
#include <unistd.h>

HttpResponseBuilder::HttpResponseBuilder() = default;
HttpResponseBuilder::~HttpResponseBuilder(){};

HttpResponse HttpResponseBuilder::build(HttpRequest const &request, Server const &server, RouteType routeType )
{
	HttpResponse response;
	const LocationBlock* block = findMatchingLocation(request.getTarget(), server);

	response.setProtocol("HTTP/1.1");
	if (request.getStatusCode() != HttpStatus::OK && request.getStatusCode() != HttpStatus::NONE)
	{
		response.setStatus(request.getStatusCode());
		buildHeader(request, response);
		return  response;
	}
	response.setStatus(HttpStatus::OK);
	buildBody(request, response, server, routeType, block);
	buildHeader(request, response);
	return response;
};


void HttpResponseBuilder::buildHeader(const HttpRequest& request, HttpResponse& response)
{		
		const std::map<std::string, std::string> headers = request.getHeader();
		auto itt = headers.find("content-type");
		if (itt != headers.end())
		{
			response.setHeader(itt->first, itt->second);
		}
	
	std::time_t time = std::time(nullptr);
	std::array<char, 100> mbstr;
	std::strftime(mbstr.data(), sizeof(mbstr), "%a, %d %b %Y %H:%M:%S GMT", std::gmtime(&time));
	response.setHeader("Date", std::string(mbstr.data()));
	response.setHeader("Server", "WebServ");
	response.setHeader("Content-Length", std::to_string(response.getBody().length()));
	
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

void HttpResponseBuilder::buildBody(const HttpRequest& request,HttpResponse& response,const Server& server,RouteType routeType, const LocationBlock* block)
{
	const std::string path = server.getRoot() + request.getTarget();


	switch (routeType)
	{
		case RouteType::STATIC_FILE:
			manageStatic(response, path ,block, server);
			break;
		case RouteType::DELETE_FILE:
			manageDelete(response, *block, request.getTarget());
			break;
		case RouteType::UPLOAD:
			if(block != nullptr)
			{
				manageUpload(response, request, *block);
			}
			else 
			{
				response.setStatus(HttpStatus::INTERNAL_SERVER_ERROR);
			}
			break;
		case RouteType::REDIRECT:
			if (block != nullptr)
			{
				manageRedirect(response, *block);
			}
			else
			{
				response.setStatus(HttpStatus::INTERNAL_SERVER_ERROR);
			}
			break;
		case RouteType::NOT_FOUND:
			response.setStatus(HttpStatus::NOT_FOUND);
			manageErrorPage(response, server);
			break;
		case RouteType::DIRECTORY_LISTING:
			manageDirectory(response, request, path);
			break;
		case RouteType::CGI:
    		response.setStatus(HttpStatus::INTERNAL_SERVER_ERROR);
			break;
		default:
			manageErrorPage(response, server);
			break;
	}
}

void HttpResponseBuilder::manageStatic(HttpResponse& response,const std::string& path, const LocationBlock* block, const Server& server)
{
		std::string filePath = path;

		if (std::filesystem::is_directory(filePath))
		{
			if (filePath.back() != '/')
			{
				filePath += "/";
			}
			if(block != nullptr && block->getIndex().has_value())
			{
				filePath += block->getIndex().value();
			}
			else if (!server.getIndex().empty())
			{
				filePath += server.getIndex();
			}
			else {
				filePath += "index.html";
			}
		}
		std::ifstream file(filePath);
		if(!file.is_open())
		{
			response.setStatus(HttpStatus::NOT_FOUND);
			manageErrorPage(response,server);
			return ;
		}
		if (access(filePath.c_str(), R_OK) != 0)
		{
 			response.setStatus(HttpStatus::FORBIDDEN);
    		return;
		}
		std::string body((std::istreambuf_iterator<char>(file)),
						  std::istreambuf_iterator<char>());
		response.setBody(body);
		response.setStatus(HttpStatus::OK);
		file.close();
}
void HttpResponseBuilder::manageDelete(HttpResponse& response, const LocationBlock& block, const std::string& target)
{
	std::string path;
	if(block.getUploadDir().has_value())
	{
		path =  block.getUploadDir().value() + target;
	}
	if (path.empty())
	{
		response.setStatus(HttpStatus::INTERNAL_SERVER_ERROR);
		return;
	}
	if (access(path.c_str(), W_OK) != 0)
	{
		response.setStatus(HttpStatus::FORBIDDEN);
		return;
	}
	int status = std::remove(path.c_str());
	if (status != 0)
	{
		response.setStatus(HttpStatus::NOT_FOUND);
		return ;
	}
	response.setStatus(HttpStatus::NO_CONTENT);
}

void HttpResponseBuilder::manageUpload(HttpResponse& response , const HttpRequest& request, const LocationBlock& block)
{
	if (block.getUploadDir().has_value())
	{
		std::string filename = std::filesystem::path(request.getTarget()).filename();
		if(access(block.getUploadDir().value().c_str(), W_OK) != 0)
		{
			response.setStatus(HttpStatus::FORBIDDEN);
			return;
		}
		std::ofstream file( block.getUploadDir().value() + "/" + filename);
		if(!file.is_open())
		{
			response.setStatus(HttpStatus::INTERNAL_SERVER_ERROR);
			return;
		}
		file << request.getBody();
		file.close();
		response.setStatus(HttpStatus::CREATED);
		return;		
	}
	response.setStatus(HttpStatus::INTERNAL_SERVER_ERROR);
}

void HttpResponseBuilder::manageRedirect(HttpResponse& response, const LocationBlock& block )
{
		if (block.getRedirectCode().has_value() && block.getRedirectUrl().has_value())
		{
			response.setStatus((HttpStatus)block.getRedirectCode().value());
			response.setHeader("Location", block.getRedirectUrl().value());
			return;
		}
		response.setStatus(HttpStatus::INTERNAL_SERVER_ERROR);
}
void HttpResponseBuilder::manageErrorPage(HttpResponse& response, const Server& server)
{
	int status = (int)response.getStatus();
	const auto& errorPages = server.getErrorPages();

	auto itt = errorPages.find(status);
	if (itt != errorPages.end())
	{
		std::ifstream file(itt->second);
		if(!file.is_open())
		{
			response.setStatus(HttpStatus::INTERNAL_SERVER_ERROR);
			return;
			}
		std::string body((std::istreambuf_iterator<char>(file)),
					 	std::istreambuf_iterator<char>());
		response.setBody(body);
	file.close();
	return;
	}
	response.setBody("<html><body><h1>" + std::to_string((int)response.getStatus()) + " Error</h1></body></html>");
}

void HttpResponseBuilder::manageDirectory(HttpResponse& response, const HttpRequest& request,const std::string& path)
{
	std::string html = "<html><body><h1>Index of " + request.getTarget() + "</h1><ul>";

	if(std::filesystem::is_directory(path))
	{
		for (const auto& entry :std::filesystem::directory_iterator(path))
		{
			std::string filename = entry.path().filename().string();
			html += "<li><a href=\"" + request.getTarget() + "/" + filename + "\">" + filename + "</a></li>";

		}
		html += "</ul></body></html>";
	}else {
	
		response.setStatus(HttpStatus::NOT_FOUND);
		return;
	}
	response.setBody(html);
	response.setStatus(HttpStatus::OK);
}
