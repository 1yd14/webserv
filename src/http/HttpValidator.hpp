/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpValidator.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 16:45:38 by rmhazres          #+#    #+#             */
/*   Updated: 2026/05/21 16:56:33 by rmhazres         ###   ########.fr       */
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
[[nodiscard]]	HttpStatus validate(const HttpRequest& request) const;
	private:
};