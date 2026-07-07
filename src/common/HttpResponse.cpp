/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 15:45:17 by rmhazres          #+#    #+#             */
/*   Updated: 2026/06/12 13:23:28 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"
#include "HttpStatus.hpp"
#include "Utils.hpp"
#include <map>
#include <string>
#include <iostream>


HttpResponse::HttpResponse()
{
	_status_code = HttpStatus::NONE;
}
HttpResponse::~HttpResponse(){};
HttpResponse::HttpResponse(const HttpResponse& other) = default;
HttpResponse& HttpResponse::operator=(const HttpResponse &other) = default;

std::string HttpResponse::getProtocol()const {return  _protocol;}
std::string HttpResponse::getBody()const {return _body;}
std::map<std::string, std::string> HttpResponse::getHeader() const {return _header;}
HttpStatus HttpResponse::getStatus()const {return  _status_code;}

void HttpResponse::setProtocol(const std::string& value){_protocol = value;}
void HttpResponse::setBody(const std::string& value){_body = value;}
void HttpResponse::setHeader(const std::string& key, const std::string& value ){_header[key] = value;}
void HttpResponse::setHeader(const std::map<std::string, std::string>&value){_header = value;}
void HttpResponse::setStatus( HttpStatus value){ _status_code = value;}


std::string HttpResponse::serialize() const
{
	std::string response;
	
	response = _protocol + " " + std::to_string((int)_status_code) + " " + getReasonPhrase(_status_code) + "\r\n";
	for(const auto& header : _header)
	{
		response += header.first + ": "	+ header.second +"\r\n";
	}
	response += "\r\n";
	response += _body;
	return response;
};