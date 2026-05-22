/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpValidator.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 11:35:43 by rmhazres          #+#    #+#             */
/*   Updated: 2026/05/22 16:18:12 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./HttpValidator.hpp"
#include <cctype>
#include <string>

HttpValidator::HttpValidator()= default;
HttpValidator::~HttpValidator(){};


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
	// status = isValidTarget(request);

	return HttpStatus::OK;
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

// HttpStatus HttpValidator::isValidTarget(const HttpRequest& request)
// {
	
// }