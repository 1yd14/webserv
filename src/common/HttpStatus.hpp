/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpStatus.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 15:57:12 by rmhazres          #+#    #+#             */
/*   Updated: 2026/07/01 12:24:25 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once


enum class HttpStatus {
		NONE = 0,
		CONTINUE = 100,
		OK = 200,
		CREATED = 201,
		NO_CONTENT = 204,
		MOVED_PERMANENTLY = 301,
		FOUND = 302,
		BAD_REQUEST = 400,
		NOT_FOUND = 404,
		METHOD_NOT_ALLOWED = 405,
		GONE = 410,
		LENGTH_REQUIRED = 411,
		PAYLOAD_TOO_LARGE = 413,
		INTERNAL_SERVER_ERROR = 500,
		NOT_IMPLEMENTED = 501,
		HTTP_VERSION_NOT_SUPPOERTED = 505,
	};