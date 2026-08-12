/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RouteType.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 12:38:23 by rmhazres          #+#    #+#             */
/*   Updated: 2026/06/04 15:06:53 by rmhazres         ###   ########.fr       */
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
