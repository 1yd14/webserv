/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpStatus.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 15:57:12 by rmhazres          #+#    #+#             */
/*   Updated: 2026/06/04 12:25:59 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once


enum class HttpStatus {
		NONE = 0,
		CONTINUE = 100,
		OK = 200,
		CREATED = 201,
		NO_CONTENT = 204,
		BAD_REQUEST = 400,
		NOT_FOUND = 404,
		GONE = 410,
		LENGTH_REQUIRED = 411,
		PAYLOAD_TOO_LARGE = 413,
		METHOD_NOT_ALLOWED = 405,
		NOT_IMPLEMENTED = 501,
		HTTP_VERSION_NOT_SUPPOERTED = 505,
	};