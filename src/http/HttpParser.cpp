/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 13:54:18 by rmhazres          #+#    #+#             */
/*   Updated: 2026/05/21 15:49:30 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./HttpParser.hpp"
#include "../common/Utils.hpp"
#include <cstddef>
#include <map>
#include <string>


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
		if (pos == std::string::npos || delim == std::string::npos)
		{
			break;
		}
		std::string key = line.substr(lineStart, delim - lineStart);
		std::string value = line.substr(delim + 2, pos - (delim + 2));
		
		key = trim(key);
		value = trim(value);

		header.insert({key, value});
		pos+=2;
	}
	req.setHeader(header);
}