/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponseBuilder.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 16:15:48 by rmhazres          #+#    #+#             */
/*   Updated: 2026/08/10 11:52:48 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponseBuilder.hpp"
#include "../common/Utils.hpp"
#include <array>
#include <bits/stdc++.h>
#include <cstdio>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <system_error>
#include <unistd.h>

HttpResponseBuilder::HttpResponseBuilder() = default;
HttpResponseBuilder::~HttpResponseBuilder(){};

HttpResponse HttpResponseBuilder::build(HttpRequest const &request, Server const &server, RouteType routeType )
{
	HttpResponse response;
	const LocationBlock* block = findMatchingLocation(request.getTarget(), server);

	response.setProtocol("HTTP/1.1");
	    // TEST: force a 500 response
    if (request.getTarget() == "/500")
    {
        sendError(response, server, HttpStatus::INTERNAL_SERVER_ERROR);
        buildHeader(request, response);
        return response;
    }
	if (request.getStatusCode() != HttpStatus::OK && request.getStatusCode() != HttpStatus::NONE)
	{
		if (request.getStatusCode() == HttpStatus::METHOD_NOT_ALLOWED)
		{
			if (block != nullptr)
			{
				std::string allow;
				for (const auto& m : block->getMethods())
				{
					allow += (allow.empty() ? "" : ", ") + m;
				}
				response.setHeader("Allow", allow);
			}
		}
		sendError(response, server, request.getStatusCode());
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
	response.setHeader("Server", "webserv");
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

std::string HttpResponseBuilder::resolvePath(const HttpRequest& request, const Server& server, const LocationBlock* block) {
	std::string path;
    std::string target = request.getTarget();
    size_t qpos = target.find('?');
    if (qpos != std::string::npos)
	{
        target = target.substr(0, qpos);
	}
    target = urlDecode(target);

    if (block != nullptr && block->getRoot().has_value())
    {
        std::string blockRoot = block->getRoot().value();
		
        if (!blockRoot.empty() && blockRoot.back() != '/')
		{
			blockRoot += '/';
		}
        std::string locationPath = block->getPath();
        path = blockRoot + target.substr(locationPath.length());
        if (!path.empty() && path.back() == '/')
		{
			path.pop_back();
		}
    }
    else
    {
		std::string root = server.getRoot();
        if (!root.empty() && root.back() != '/')
		{
			root += '/';
		}
        path = root + target.substr(1);
    }
	return path;
}


void HttpResponseBuilder::buildBody(const HttpRequest& request, HttpResponse& response,const Server& server,RouteType routeType, const LocationBlock* block)
{
	
	std::string path = resolvePath(request, server, block);

	switch (routeType)
	{
		case RouteType::STATIC_FILE:
			manageStatic(response, path ,block, server);
			break;
		case RouteType::DELETE_FILE:
			manageDelete(response, *block, request.getTarget(), server);
			break;
		case RouteType::UPLOAD:
			if(block != nullptr)
			{
				manageUpload(response, request, *block, server);
			}
			else 
			{
				sendError(response, server, HttpStatus::INTERNAL_SERVER_ERROR);
			}
			break;
		case RouteType::REDIRECT:
			if (block != nullptr)
			{
				manageRedirect(response, *block, server);
			}
			else
			{
				sendError(response, server, HttpStatus::INTERNAL_SERVER_ERROR);
			}
			break;
		case RouteType::NOT_FOUND:
			sendError(response, server, HttpStatus::NOT_FOUND);
			break;
		case RouteType::DIRECTORY_LISTING:
			manageDirectory(response, request, path, server);
			break;
		default:
			sendError(response, server, HttpStatus::INTERNAL_SERVER_ERROR);
			break;
	}
}

void HttpResponseBuilder::sendError(HttpResponse& response, const Server& server, HttpStatus status) {
	response.setStatus(status);
	//manageErrorPage(response, server);
	getErrorBody(response, server);
}

void HttpResponseBuilder::manageStatic(HttpResponse& response,const std::string& path, const LocationBlock* block, const Server& server)
{
		std::string filePath = path;
		bool isDir = false;
		std::error_code ec;
		isDir = std::filesystem::is_directory(filePath, ec);
		if (ec)
		{
			sendError(response, server, HttpStatus::NOT_FOUND);
			return;
		}
		if (isDir)
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
		if (access(filePath.c_str(), R_OK) != 0)
		{
			sendError(response, server, HttpStatus::FORBIDDEN);
			return ;
		}
		if(!file.is_open())
		{
			sendError(response, server, HttpStatus::NOT_FOUND);
			return ;
		}
		std::string body((std::istreambuf_iterator<char>(file)),
						  std::istreambuf_iterator<char>());
		response.setBody(body);
		response.setStatus(HttpStatus::OK);
		response.setHeader("Content-Type", getMimeType(filePath));
}

void HttpResponseBuilder::manageDelete(HttpResponse& response, const LocationBlock& block, const std::string& target, const Server& server)
{
	std::string path;
	if(block.getUploadDir().has_value())
	{
		std::string blockPath = block.getPath();
		std::string filename = target.substr(blockPath.length());
		path = block.getUploadDir().value() + filename;
	}
	if (path.empty())
	{
		sendError(response, server, HttpStatus::INTERNAL_SERVER_ERROR);
		return;
	}
	if (access(path.c_str(), F_OK) != 0)
	{
		sendError(response, server, HttpStatus::NOT_FOUND);
		return;
	}
	if (access(path.c_str(), W_OK) != 0)
	{
		sendError(response, server, HttpStatus::FORBIDDEN);
		return;
	}
	int status = std::remove(path.c_str());
	if (status != 0)
	{
		sendError(response, server, HttpStatus::NOT_FOUND);
		return ;
	}
	sendError(response, server, HttpStatus::NO_CONTENT);
}

void HttpResponseBuilder::manageUpload(HttpResponse& response , const HttpRequest& request, const LocationBlock& block, const Server& server)
{
	
	if (block.getUploadDir().has_value())
	{
		std::string uploadDir = block.getUploadDir().value();
		
		std::string filename = std::filesystem::path(request.getTarget()).filename();
		if(access(uploadDir.c_str(), W_OK) != 0)
		{
			sendError(response, server, HttpStatus::FORBIDDEN);
			return;
		}
		std::ofstream file( uploadDir + "/" + filename);
		if(!file.is_open())
		{
			sendError(response, server, HttpStatus::INTERNAL_SERVER_ERROR);
			return;
		}
		
		const std::map<std::string, std::string> headers = request.getHeader();
		auto it = headers.find("content-type");
		if(it != headers.end() && it->second.find("multipart/form-data") != std::string::npos)
		{
			if(!parseFormData(response, request , uploadDir))
			{
				sendError(response, server, HttpStatus::INTERNAL_SERVER_ERROR);
				return;
			}
			
			response.setStatus(HttpStatus::CREATED);
			return;
		}
		file << request.getBody();
		response.setStatus(HttpStatus::CREATED);
		return;		
	}
	sendError(response, server, HttpStatus::INTERNAL_SERVER_ERROR);
}

bool HttpResponseBuilder::parseFormData(HttpResponse& response, const HttpRequest& request, const std::string& uploadDir)
{
	std::map<std::string, std::string> headers = request.getHeader();
	std::string boundary;
	std::string filename;
	(void)response;
	for (const auto& header : headers)
	{
		if(header.first == "content-type")
		{
			boundary = header.second;
		}
	}
	size_t eq = boundary.find("=");
	if(eq == std::string::npos)
	{
		return false;
	}
	boundary = boundary.substr(eq +1);
	std::string body = request.getBody();
	
	body = body.substr(body.find("--" + boundary));
	size_t feq = body.find("filename=");
	if(feq == std::string::npos)
	{
		return false;
	}
	filename = body.substr(feq + 10, (body.find("Content-Type")  - (feq + 13) ));
	
	size_t dataStart = body.find("\r\n\r\n");
	size_t dataEnd = body.find("\r\n--" + boundary + "--");

	if(dataStart == std::string::npos || dataEnd == std::string::npos)
	{
		return false;
	}
	
	std::string data  = body.substr(dataStart + 4, dataEnd -( dataStart + 4));
	std::ofstream outFile (uploadDir + "/" + filename, std::ios::binary);
	outFile.write(data.c_str(), data.size());
	outFile.close();
	return true;
}

void HttpResponseBuilder::manageRedirect(HttpResponse& response, const LocationBlock& block, const Server& server)
{
	if (block.getRedirectCode().has_value() && block.getRedirectUrl().has_value())
	{
		response.setStatus((HttpStatus)block.getRedirectCode().value());
		response.setHeader("Location", block.getRedirectUrl().value());
		return;
	}
	sendError(response, server, HttpStatus::INTERNAL_SERVER_ERROR);
}

void HttpResponseBuilder::manageDirectory(HttpResponse& response, const HttpRequest& request,const std::string& path, const Server& server)
{
	std::string html = "<html><body><h1>Index of " + request.getTarget() + "</h1><ul>";
	std::error_code ec;
	bool isDir = false;

	isDir = std::filesystem::is_directory(path, ec);
	if (ec) 
	{
		sendError(response, server, HttpStatus::NOT_FOUND);
    	return;
	}
	if(isDir)
	{
		for (const auto& entry :std::filesystem::directory_iterator(path))
		{
			std::string filename = entry.path().filename().string();
			html += "<li><a href=\"" + request.getTarget() + "/" + filename + "\">" + filename + "</a></li>";

		}
		html += "</ul></body></html>";
	}else {
	
		sendError(response, server, HttpStatus::NOT_FOUND);
		return;
	}
	response.setBody(html);
	response.setStatus(HttpStatus::OK);
}
