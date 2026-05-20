/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 13:54:18 by rmhazres          #+#    #+#             */
/*   Updated: 2026/05/20 16:06:57 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./HttpParser.hpp"
#include <cstddef>
#include <map>
#include <ostream>
#include <string>
#include <iostream>


HttpParser::HttpParser() = default;
HttpParser::~HttpParser(){};


HttpRequest HttpParser::parseHttp(const std::string &rawRequest) const
{
	HttpRequest request;

	
	if (rawRequest.empty())
	{
		request.setStatusCode(HttpStatus::BAD_REQUEST);
		return request;	
	}
	// extracting first line from the request
	size_t firstLine = rawRequest.find("\r\n");
	if (firstLine == std::string::npos)
	{
		request.setStatusCode(HttpStatus::BAD_REQUEST);
		return request;
	}
	extractFirstLine(rawRequest.substr(0, firstLine), request);
	
	// extracting the header from the request

	size_t emptyHeaderLine = rawRequest.find("\r\n\r\n");
	if (emptyHeaderLine == std::string::npos)
	{
		request.setStatusCode(HttpStatus::BAD_REQUEST);
		return request;
	}
	extractHeaders(rawRequest.substr(firstLine + 2, emptyHeaderLine - firstLine ), request);
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
	
	std::map<std::string, std::string> header;
	// std::cout << line << std::endl;
	
	size_t pos = 0;
	size_t delim = 0;
	size_t lineStart = 0;
	while(pos < line.length())
	{
		lineStart = pos;
		pos = line.find("\r\n", lineStart);
		delim = line.find(":", lineStart);
		if (pos == std::string::npos || pos == std::string::npos)
		{
			break;
		}
		std::string key = line.substr(lineStart, delim - lineStart);
		std::string value = line.substr(delim + 2, pos - (delim + 2));
		
		size_t trim = key.find_last_of(" ");
		if (trim != std::string::npos)
		{
			key = key.substr(0, trim);
		}
		trim = value.find_first_of(" ");
		if (trim != std::string::npos)
		{
			value = value.substr(0, trim);
		}

		header.insert({key, value});
		pos+=2;
	}
	req.setHeader(header);
}

