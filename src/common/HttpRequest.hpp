/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 15:20:41 by rmhazres          #+#    #+#             */
/*   Updated: 2026/05/27 13:52:34 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once

#include "HttpStatus.hpp"
#include <map>
#include <string>


class HttpRequest 
{
	public:
		HttpRequest();
		~HttpRequest();
		HttpRequest(const HttpRequest &other);
		HttpRequest& operator=(const HttpRequest &other);

[[nodiscard]]	std::string getMethod() const;
[[nodiscard]]	std::string getTarget() const;
[[nodiscard]]	std::string getProtocol() const;
[[nodiscard]]	std::map<std::string, std::string> getHeader() const;
[[nodiscard]]	std::string getBody() const;
[[nodiscard]]	HttpStatus getStatusCode() const;
[[nodiscard]]	long getContentLength() const;

		void setMethod(const std::string& value);
		void setTarget(const std::string& value);
		void setProtocol(const std::string& value);
		void setHeader(const std::map<std::string, std::string>& value);
		void setBody(const std::string& value);
		void setStatusCode(HttpStatus code);
		void setContentLength(const long& value);

	private:
		std::string _method;
		std::string _target;
		std::string _protocol;
		std::map<std::string, std::string> _header;
		std::string _body;
		long _contentLength;
		HttpStatus _statusCode;
		
};