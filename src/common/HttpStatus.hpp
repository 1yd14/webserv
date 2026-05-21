/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpStatus.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 15:57:12 by rmhazres          #+#    #+#             */
/*   Updated: 2026/05/21 15:46:48 by rmhazres         ###   ########.fr       */
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
	};