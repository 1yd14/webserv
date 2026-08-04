/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 13:54:18 by rmhazres          #+#    #+#             */
/*   Updated: 2026/08/04 13:52:10 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./HttpParser.hpp"
#include "../common/Utils.hpp"
#include <cctype>
#include <climits>
#include <cstddef>
#include <map>
#include <string>
#include <iostream>

HttpParser::HttpParser() = default;
HttpParser::~HttpParser(){};


HttpRequest HttpParser::parseHttp(const std::string &rawRequest)
{
	HttpRequest request;

	
	if (rawRequest.empty())
	{
		request.setStatusCode(HttpStatus::BAD_REQUEST);
		return request;	
	}
	
	size_t firstLine = rawRequest.find("\r\n");
	if (firstLine == std::string::npos)
	{
		request.setStatusCode(HttpStatus::BAD_REQUEST);
		return request;
	}

	extractFirstLine(rawRequest.substr(0, firstLine), request);

	std::string target = request.getTarget();
		if (target.find("http://") == 0 || target.find("https://") == 0)
		{
			size_t pathStart = target.find('/', 7);
			request.setTarget(pathStart == std::string::npos ? "/" : target.substr(pathStart));
		}
		
	size_t emptyHeaderLine = rawRequest.find("\r\n\r\n");
	if (emptyHeaderLine == std::string::npos)
	{
		request.setStatusCode(HttpStatus::BAD_REQUEST);
		return request;
	}
	extractHeaders(rawRequest.substr(firstLine + 2, emptyHeaderLine - firstLine ), request);
	extractContentLength(request);


	request.setBody(trim(rawRequest.substr(emptyHeaderLine + 4)));
	
	return request;
}

void HttpParser::extractFirstLine(const std::string &line, HttpRequest &req)
{

	size_t first = line.find(" ");
	size_t second = line.find(" ", first +1 );

	if (first == std::string::npos || second == std::string::npos)
	{
		req.setStatusCode(HttpStatus::BAD_REQUEST);
		return;
	}
	req.setMethod(line.substr(0, first));
	req.setTarget(line.substr(first + 1,second - (first + 1 ) ));
	req.setProtocol(line.substr(second+ 1));
}

void HttpParser::extractHeaders(const std::string &line, HttpRequest &req)
{
	req.setHeader(parseHeaders(line));
}

void HttpParser::extractContentLength(HttpRequest& req)
{
	const auto& header = req.getHeader();
	auto itt = header.find("content-length");
	
	if(itt != header.end())
	{
		req.setContentLength(safeConvertLong(itt->second));
	}
}
