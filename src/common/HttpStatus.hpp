/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpStatus.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 15:57:12 by rmhazres          #+#    #+#             */
/*   Updated: 2026/05/26 16:54:45 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once


enum class HttpStatus {
		NONE = 0,
		CONTINUE = 100,
		OK = 200,
		CREATED = 201,
		BAD_REQUEST = 400,
		NOT_FOUND = 404,
		LENGTH_REQUIRED = 411,
		METHOD_NOT_ALLOWED = 405,
		NOT_IMPLEMENTED = 501,
		HTTP_VERSION_NOT_SUPPOERTED = 505,

	};