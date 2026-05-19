/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 13:54:18 by rmhazres          #+#    #+#             */
/*   Updated: 2026/05/19 17:34:20 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "./HttpParser.hpp"
#include <cstddef>
#include <string>


HttpParser::HttpParser() = default;
HttpParser::~HttpParser(){};


//temp until statuscode file get merged
const int BAD_REQUEST = 400;
// to be deleted

HttpRequest HttpParser::parseHttp(const std::string &rawRequest) const
{
	HttpRequest request;


	
	if (rawRequest.empty())
	{
		request.setStatusCode(BAD_REQUEST);
		return request;	
	}
	size_t firstLine = rawRequest.find("\r\n");
	if (firstLine == std::string::npos)
	{
		request.setStatusCode(BAD_REQUEST);
		return request;
	}
	extractFirstLine(rawRequest.substr(0, firstLine), request);
	
}

static void extractFirstLine(const std::string &line, HttpRequest &req)
{
	size_t first = line.find(" ");
	size_t second = line.find(" ", first +1 );

	if (first == std::string::npos || second == std::string::npos || third == std::string::npos)
	{
		req.setStatusCode(BAD_REQUEST);
		return;
	}
	req.setMethod(line.substr(0, first));
	req.setTarget(line.substr(first + 1,second - (first +1 ) ));
	req.setProtocol(line.substr(second+ 1));
}