/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RouteType.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 12:38:23 by rmhazres          #+#    #+#             */
/*   Updated: 2026/06/03 13:05:11 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

enum class RouteType{
		STATIC_FILE,
    	UPLOAD,
    	DELETE_FILE,
    	CGI,
    	DIRECTORY_LISTING,
    	NOT_FOUND,
		REDIRECT
};