/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 12:50:57 by rmhazres          #+#    #+#             */
/*   Updated: 2026/07/13 10:26:50 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Router.hpp"
#include <filesystem>
#include <string>

#include "../common/Utils.hpp"

RouteType Router::route(const HttpRequest& request, const Server &server) const
{
	const std::string target = request.getTarget();
	const LocationBlock* block = findMatchingLocation(target, server);

	if (block == nullptr)
	{
		return RouteType::NOT_FOUND;
	}
	if (block->getRedirectCode().has_value())
	{		
			return RouteType::REDIRECT;
	}
	if (block->getCgiExtension().has_value())
	{
			return RouteType::CGI;
	}
	if (block->getUploadDir().has_value() && (request.getMethod() == "POST"))
	{
			return RouteType::UPLOAD;
	}
	if (request.getMethod() == "DELETE")
	{
		return RouteType::DELETE_FILE;
	}
	if (block->getAutoIndex().has_value() && block->getAutoIndex().value())
	{
	
	if(std::filesystem::is_directory(server.getRoot() + target))
	{
		return RouteType::DIRECTORY_LISTING;
	}
	}
	return RouteType::STATIC_FILE;
};
