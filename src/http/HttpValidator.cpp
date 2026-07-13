/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpValidator.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 11:35:43 by rmhazres          #+#    #+#             */
/*   Updated: 2026/07/10 18:14:52 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./HttpValidator.hpp"
#include <cctype>
#include <cstddef>
#include <string>
#include <vector>
#include <algorithm>
#include "../common/Utils.hpp"
#include <iostream>

HttpValidator::HttpValidator()= default;
HttpValidator::~HttpValidator(){};


// Main entry point to the validator
HttpStatus HttpValidator::validate(HttpRequest& request,const Server& server) const
{
	HttpStatus status;
	
	status = HttpStatus::OK;
	status = isValidMethod(request);
	if (status != HttpStatus::OK)
	{
		request.setStatusCode(status);
		return status;
	}
	status = isValidTarget(request, server);
	if (status != HttpStatus::OK)
	{
		request.setStatusCode(status);
		return status;
	}
	status = isValidProtocol(request);
	{
		if (status != HttpStatus::OK)
		{
			request.setStatusCode(status);
			return status;
		}
	}
	status = isValidHeader(request);
	{
		if (status != HttpStatus::OK)
		{
			request.setStatusCode(status);
			return  status;
		}
	}
	status = isValidBody(request, server.getMaxBodySize());
	{
		if (status != HttpStatus::OK)
		{
			request.setStatusCode(status);
			return status;
		}
	}
	return status;
};

HttpStatus HttpValidator::isValidMethod(const HttpRequest& request)
{
	const std::string method = request.getMethod();
	if (method == "GET" || method == "POST" || method == "DELETE" || method == "HEAD")
	{
		return HttpStatus::OK;
	}
	if (method == "OPTIONS" || method == "PUT" ||
		 method == "TRACE" || method == "PATCH" || method == "CONNECT")
	{
		
    	return HttpStatus::METHOD_NOT_ALLOWED;
	}
	for (const auto &cha : method)
	{
		if (isupper(cha) ==  0)
		{
			return HttpStatus::BAD_REQUEST;
		}
	}
	return HttpStatus::METHOD_NOT_ALLOWED;
}

HttpStatus HttpValidator::isValidTarget(const HttpRequest& request, const Server& server)
{
	std::string target = request.getTarget();
	
	if(target.length() > 2048)
	{
		return HttpStatus::URI_TOO_LONG;
	}
	if (target.empty() || target[0] != '/')
	{
		return HttpStatus::BAD_REQUEST;
	}
	if (unsafeCharCheck(target) == HttpStatus::BAD_REQUEST)
	{
		return  HttpStatus::BAD_REQUEST;
	}
	const LocationBlock* block = findMatchingLocation(target, server);
	if (block != nullptr)
	{
		std::vector<std::string> methods = block->getMethods();
		if (methods.empty())
		{
			return HttpStatus::OK;
		}
		std::string methodToCheck = request.getMethod() == "HEAD" ? "GET" : request.getMethod();

		auto itt = std::find(methods.begin(), methods.end(),methodToCheck);
		if (itt == methods.end())
		{
			return (HttpStatus::METHOD_NOT_ALLOWED);
		}
	}
	return HttpStatus::OK;
}

HttpStatus HttpValidator::unsafeCharCheck(const std::string& target)
{
	const std::string unsafe = "<>#%{}|\\^~[] ";

	for (const auto &cha : unsafe)
	{
		if (target.find(cha) != std::string::npos)
		{
			return HttpStatus::BAD_REQUEST;
		}
	}
	size_t found = target.find("..");
	if (found != std::string::npos)
	{
		return HttpStatus::BAD_REQUEST;
	}
	return HttpStatus::OK;
}

HttpStatus HttpValidator::isValidProtocol(const HttpRequest& request)
{
	const std::string protocol = request.getProtocol();
	
	if (protocol.empty())
	{
		return HttpStatus::BAD_REQUEST;
	}
	
	if (protocol.compare(0,5, "HTTP/") != 0)
	{
		return HttpStatus::BAD_REQUEST;
	}
		
	if (protocol != "HTTP/1.0" && protocol != "HTTP/1.1")
	{
		return HttpStatus::HTTP_VERSION_NOT_SUPPOERTED;
	}
	return HttpStatus::OK;
}

HttpStatus HttpValidator::isValidHeader(const HttpRequest& request)
{
	std::map<std::string, std::string> header = request.getHeader();
	
	if (request.getProtocol().ends_with("1"))
	{
		if(!header.contains("host") || header.at("host").empty())
		{
			return HttpStatus::BAD_REQUEST;
		}
	}
	if (request.getMethod() == "POST")
	{
		if(!header.contains("content-length") || header.at("content-length").empty())
		{
			return HttpStatus::LENGTH_REQUIRED;
		}
	}
	return HttpStatus::OK;
}
HttpStatus HttpValidator::isValidBody(const HttpRequest& request,size_t max_size)
{
	if(request.getMethod() != "POST")
	{
		return HttpStatus::OK;
	}
	if (request.getContentLength() == -1)
	{
		return HttpStatus::BAD_REQUEST;
	}
	if (request.getBody().length() != (size_t)request.getContentLength())
	{		
		return HttpStatus::BAD_REQUEST;
	}
	if(request.getBody().length() > max_size)
	{
		return HttpStatus::PAYLOAD_TOO_LARGE;
	}
	return HttpStatus::OK;
}
