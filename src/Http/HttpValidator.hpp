/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpValidator.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 16:45:38 by rmhazres          #+#    #+#             */
/*   Updated: 2026/08/12 12:36:41 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../Common/HttpRequest.hpp"
#include "../Config/Server.hpp"
#include <cstddef>

class HttpValidator
{
	public:
				HttpValidator();
				~HttpValidator();
				HttpValidator(const HttpValidator &other) = delete;
				HttpValidator& operator=(const HttpValidator &other) = delete;
[[nodiscard]]	HttpStatus validate(HttpRequest& request, const Server& server) const;
	private:
				static HttpStatus isValidMethod(const HttpRequest& request);
				static HttpStatus isValidTarget(const HttpRequest& request,const Server& server);
				static HttpStatus isValidProtocol(const HttpRequest& request);
				static HttpStatus unsafeCharCheck(const std::string& target);
				static HttpStatus isValidHeader(const HttpRequest& request);
				static HttpStatus isValidBody(const HttpRequest& request, size_t max_size);
};