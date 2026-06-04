/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 15:45:17 by rmhazres          #+#    #+#             */
/*   Updated: 2026/06/04 10:06:00 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"
#include "HttpStatus.hpp"
#include <string>



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
void HttpResponse::setHeader(const std::string& key, const std::string& value ){_header.insert(key, value);}
void HttpResponse::setStatus( HttpStatus value){ _status_code = value;}