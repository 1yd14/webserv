/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 12:50:57 by rmhazres          #+#    #+#             */
/*   Updated: 2026/06/10 12:17:46 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Router.hpp"
#include <filesystem>
#include <vector>


RouteType Router::route(const HttpRequest& request, const Server &server) const
{
	const std::string target = request.getTarget();
	std::vector<LocationBlock> blocks =  server.getLocationBlocks();

	for (const auto& location : blocks) 
	{
		if(target.find(location.getPath()) == 0)
		{
			if (location.getRedirectCode().has_value())
			{
				return RouteType::REDIRECT;
			}
			if (location.getCgiExtension().has_value())
			{
				return RouteType::CGI;
			}
			if (location.getUploadDir().has_value() && (request.getMethod() == "POST"))
			{
				return RouteType::UPLOAD;
			}
			if (request.getMethod() == "DELETE")
			{
				return RouteType::DELETE_FILE;
			}
			if (location.getAutoIndex().has_value() && location.getAutoIndex().value())
			{
				if(std::filesystem::is_directory(server.getRoot() + target))
				{
					return RouteType::DIRECTORY_LISTING;
				}
			}
			return RouteType::STATIC_FILE;
		}
	}
	return RouteType::NOT_FOUND; 
};
