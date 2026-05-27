/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 13:54:18 by rmhazres          #+#    #+#             */
/*   Updated: 2026/05/27 14:52:44 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./HttpParser.hpp"
#include "../common/Utils.hpp"
#include <cctype>
#include <climits>
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
	
	// extracting first line from the raw request string
	size_t firstLine = rawRequest.find("\r\n");
	if (firstLine == std::string::npos)
	{
		request.setStatusCode(HttpStatus::BAD_REQUEST);
		return request;
	}
	extractFirstLine(rawRequest.substr(0, firstLine), request);
	
	// extracting the header from the raw request string
	size_t emptyHeaderLine = rawRequest.find("\r\n\r\n");
	if (emptyHeaderLine == std::string::npos)
	{
		request.setStatusCode(HttpStatus::BAD_REQUEST);
		return request;
	}
	extractHeaders(rawRequest.substr(firstLine + 2, emptyHeaderLine - firstLine ), request);

	// extracting the body from the raw request string
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
	
	std::map<std::string, std::string> header;

	size_t pos = 0;
	size_t delim = 0;
	size_t lineStart = 0;
	while(pos < line.length())
	{
		lineStart = pos;
		pos = line.find("\r\n", lineStart);
		delim = line.find(":", lineStart);
		std::cout << delim << " > " << pos << std::endl;
		if (pos == std::string::npos || delim == std::string::npos || delim > pos )
		{
			break;
		}
		std::string key = line.substr(lineStart, delim - lineStart);
		std::string value;
		
		key = trim(key);
		if (delim + 2 < pos)
		{
			value = trim(line.substr(delim + 2, pos - (delim + 2)));
		}
		if (compareStr(key , "content-length") == 0)
		{
			convertContentLength(value , req);
		}
		header.insert({key, value});
		pos+=2;
	}
	req.setHeader(header);
}


void HttpParser::convertContentLength(const std::string& str, HttpRequest& req)
{
	const int msize = 19;
	if (str.empty())
	{
		return;
	}
	if (str.length() == msize && str > std::to_string(LONG_MAX))
	{
		req.setContentLength(-1);
		return;
	}
	for (char cha : str)
	{
		if(!(bool)isdigit(cha))
		{
			req.setContentLength(-1);
			return;
		}
	}
	req.setContentLength(std::stol(str));
}
