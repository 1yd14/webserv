/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpStatus.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 15:57:12 by rmhazres          #+#    #+#             */
/*   Updated: 2026/05/22 15:55:48 by rmhazres         ###   ########.fr       */
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
		METHOD_NOT_ALLOWED = 405,
		NOT_IMPLEMENTED = 501,
	};