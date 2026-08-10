/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpValidator.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 16:45:38 by rmhazres          #+#    #+#             */
/*   Updated: 2026/08/10 14:55:46 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../common/HttpRequest.hpp"
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