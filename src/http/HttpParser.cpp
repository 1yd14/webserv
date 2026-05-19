/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 13:54:18 by rmhazres          #+#    #+#             */
/*   Updated: 2026/05/19 15:58:01 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "./HttpParser.hpp"
#include <cstddef>
#include <string>

HttpParser::HttpParser() = default;
HttpParser::~HttpParser(){};


HttpRequest HttpParser::parseHttp(const std::string &rawRequest) const
{
	HttpRequest request;
	
	if (rawRequest.empty())
	{
		request.setStatusCode(400);
		return request;	
	}
	size_t firstLine = rawRequest.find("\r\n");
	if (firstLine == std::string::npos)
	{
		request.setStatusCode(400);
		return request;
	}
	extractFirstLine(rawRequest.substr(0, firstLine));
	
}