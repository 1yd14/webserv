/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 15:20:46 by rmhazres          #+#    #+#             */
/*   Updated: 2026/05/20 16:24:06 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"
#include "HttpStatus.hpp"
#include <map>
#include <string>

HttpRequest::HttpRequest() = default;
HttpRequest::~HttpRequest()
{}
HttpRequest::HttpRequest(const HttpRequest &other) = default;
HttpRequest& HttpRequest::operator=(const HttpRequest &other)
{
	if (this != &other)
	{
		_method = other._method;
		_target = other._target;
		_protocol = other._protocol;
		_header = other._header;
		_body = other._body;
		_statusCode = other._statusCode;
	}
	return *this;
}

// class getters
std::string HttpRequest::getMethod() const {return _method;}
std::string HttpRequest::getTarget() const {return _target;}
std::string HttpRequest::getProtocol() const {return _protocol;}
std::map<std::string, std::string> HttpRequest::getHeader() const {return _header;}
std::string HttpRequest::getBody() const {return _body;}
HttpStatus	HttpRequest::getStatusCode()const {return _statusCode;}

// class setters
void HttpRequest::setMethod(const std::string& value){_method = value;}
void HttpRequest::setTarget(const std::string& value){_target = value;}
void HttpRequest::setProtocol(const std::string &value){_protocol = value;}
void HttpRequest::setHeader(const std::map<std::string, std::string> &value){_header = value;}
void HttpRequest::setBody(const std::string& value){_body = value;}
void HttpRequest::setStatusCode(HttpStatus code) {_statusCode = code;}