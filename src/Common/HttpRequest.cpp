/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 15:20:46 by rmhazres          #+#    #+#             */
/*   Updated: 2026/08/12 11:01:56 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"
#include "HttpStatus.hpp"
#include <map>
#include <string>

HttpRequest::HttpRequest()
{
	_statusCode = HttpStatus::NONE;
	_contentLength = 0;
}

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
		_contentLength = other._contentLength;
	}
	return *this;
}

// class getters
std::string HttpRequest::getMethod() const {return _method;}
std::string HttpRequest::getTarget() const {return _target;}
std::string HttpRequest::getProtocol() const {return _protocol;}
std::map<std::string, std::string> HttpRequest::getHeader() const {return _header;}
std::string HttpRequest::getBody() const {return _body;}
long HttpRequest::getContentLength() const {return _contentLength;}
HttpStatus	HttpRequest::getStatusCode()const {return _statusCode;}

// class setters
void HttpRequest::setMethod(const std::string& value){_method = value;}
void HttpRequest::setTarget(const std::string& value){_target = value;}
void HttpRequest::setProtocol(const std::string &value){_protocol = value;}
void HttpRequest::setHeader(const std::map<std::string, std::string> &value){_header = value;}
void HttpRequest::setBody(const std::string& value){_body = value;}
void HttpRequest::setStatusCode(HttpStatus code) {_statusCode = code;}
void HttpRequest::setContentLength(const long& value){_contentLength = value;}