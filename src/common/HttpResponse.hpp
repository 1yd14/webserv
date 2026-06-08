/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 15:04:24 by rmhazres          #+#    #+#             */
/*   Updated: 2026/06/08 18:00:18 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <map>
#include <string>
#include "HttpStatus.hpp"

class HttpResponse
{
	public:
					HttpResponse();
					~HttpResponse();
					HttpResponse(const HttpResponse &other);
					HttpResponse& operator=(const HttpResponse &other);

[[nodiscard]]		std::string getProtocol() const;
[[nodiscard]]		HttpStatus getStatus() const;
[[nodiscard]]		std::map<std::string, std::string> getHeader() const;
[[nodiscard]]		std::string getBody()const;
		
					void setProtocol(const std::string& value);
					void setStatus(HttpStatus value);
					void setHeader(const std::string& key,const std::string& value);
					void setHeader(const std::map<std::string, std::string>& value);
					void setBody(const std::string &value);
	private:
					std::string _protocol;
					HttpStatus	_status_code;
					std::map<std::string, std::string> _header;
					std::string _body;
};