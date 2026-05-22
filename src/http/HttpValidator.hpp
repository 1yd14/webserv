/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpValidator.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 16:45:38 by rmhazres          #+#    #+#             */
/*   Updated: 2026/05/22 16:11:41 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../common/HttpRequest.hpp"

class HttpValidator
{
	public:
				HttpValidator();
				~HttpValidator();
				HttpValidator(const HttpValidator &other) = delete;
				HttpValidator& operator=(const HttpValidator &other) = delete;
[[nodiscard]]	HttpStatus validate( HttpRequest& request) const;
	private:
				static HttpStatus isValidMethod(const HttpRequest& request);
				static HttpStatus isValidTarget(const HttpRequest& request);
};