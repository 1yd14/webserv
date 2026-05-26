/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpValidator.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 11:35:43 by rmhazres          #+#    #+#             */
/*   Updated: 2026/05/26 16:29:58 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./HttpValidator.hpp"
#include <cctype>
#include <cstddef>
#include <string>
#include <iostream>
HttpValidator::HttpValidator()= default;
HttpValidator::~HttpValidator(){};


// Main entry point to the validator
HttpStatus HttpValidator::validate(HttpRequest& request) const
{
	HttpStatus status;
	
	status = HttpStatus::OK;
	status = isValidMethod(request);
	if (status != HttpStatus::OK)
	{
		request.setStatusCode(status);
		return status;
	}
	status = isValidTarget(request);
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
	return status;
};

HttpStatus HttpValidator::isValidMethod(const HttpRequest& request)
{
	const std::string method = request.getMethod();
	
	if (method == "GET" || method == "POST" || method == "DELETE")
	{
		return HttpStatus::OK;
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

HttpStatus HttpValidator::isValidTarget(const HttpRequest& request)
{
	std::string target = request.getTarget();
	
	if (target.empty() || !target.starts_with('/'))
	{
		return HttpStatus::BAD_REQUEST;
	}
	return unsafeCharCheck(target);
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
	const int str_size = 8;
	const int first_half = 5;
	
	if (protocol.empty() || protocol.size() != str_size)
	{
		return HttpStatus::BAD_REQUEST;
	}
	if (protocol.compare(0,first_half,"HTTP/") != 0)
	{
		return HttpStatus::BAD_REQUEST;
	}
	if (protocol.compare(first_half , 3,"1.0") != 0 
		&& protocol.compare(first_half , 3,"1.1") != 0 )
	{
			return HttpStatus::HTTP_VERSION_NOT_SUPPOERTED;
	}
	return HttpStatus::OK;
}

