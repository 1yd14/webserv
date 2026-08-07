/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponseBuilder.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 16:15:48 by rmhazres          #+#    #+#             */
/*   Updated: 2026/08/07 15:04:35 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponseBuilder.hpp"
#include "../common/Utils.hpp"
#include <array>
#include <cstddef>
#include <ctime>
#include <exception>
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

void HttpResponseBuilder::buildBody(const HttpRequest& request,HttpResponse& response,const Server& server,RouteType routeType, const LocationBlock* block)
{
	
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
        std::string locationPath = findMatchingLocation(request.getTarget(), server)->getPath();
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
		bool isDir = false;
		try {
		  isDir = std::filesystem::is_directory(filePath);
		}catch (const std::filesystem::filesystem_error&) {
			response.setStatus(HttpStatus::NOT_FOUND);
			manageErrorPage(response, server);
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
		try {
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
			response.setHeader("Content-Type", getMimeType(filePath));
			file.close();
			
		} catch (const std::exception& e) {
			    std::cout << "manageStatic exception: " << e.what() << "\n";

			response.setStatus(HttpStatus::NOT_FOUND);
			manageErrorPage(response, server);
    		return;
		}
}
void HttpResponseBuilder::manageDelete(HttpResponse& response, const LocationBlock& block, const std::string& target)
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
		response.setStatus(HttpStatus::INTERNAL_SERVER_ERROR);
		return;
	}
	
	if (access(path.c_str(), F_OK) != 0)
	{
		response.setStatus(HttpStatus::NOT_FOUND);
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
		std::string uploadDir = block.getUploadDir().value();
		
		std::string filename = std::filesystem::path(request.getTarget()).filename();
		if(access(uploadDir.c_str(), W_OK) != 0)
		{
			response.setStatus(HttpStatus::FORBIDDEN);
			return;
		}
		std::ofstream file( uploadDir + "/" + filename);
		if(!file.is_open())
		{
			response.setStatus(HttpStatus::INTERNAL_SERVER_ERROR);
			return;
		}
		
		const std::map<std::string, std::string> headers = request.getHeader();
		auto it = headers.find("content-type");
		if(it != headers.end() && it->second.find("multipart/form-data") != std::string::npos)
		{
			if(!parseFormData(response, request , uploadDir))
			{
				response.setStatus(HttpStatus::INTERNAL_SERVER_ERROR);
				return;
			}
			
			response.setStatus(HttpStatus::CREATED);
			return;
		}
		file << request.getBody();
		file.close();
		response.setStatus(HttpStatus::CREATED);
		return;		
	}
	response.setStatus(HttpStatus::INTERNAL_SERVER_ERROR);
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

	bool isDir = false;

	try {
	isDir = std::filesystem::is_directory(path);
	} catch (const std::filesystem::filesystem_error&) {
	 	response.setStatus(HttpStatus::NOT_FOUND);
   		// manageErrorPage(response, server);
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
	
		response.setStatus(HttpStatus::NOT_FOUND);
		return;
	}
	response.setBody(html);
	response.setStatus(HttpStatus::OK);
}
