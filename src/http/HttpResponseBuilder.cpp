/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponseBuilder.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 16:15:48 by rmhazres          #+#    #+#             */
/*   Updated: 2026/06/03 16:32:28 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponseBuilder.hpp"
#include <filesystem>


HttpResponseBuilder::HttpResponseBuilder() = default;
HttpResponseBuilder::~HttpResponseBuilder(){};

HttpResponse HttpResponseBuilder::build(HttpRequest const &request, Server const &server, RouteType routeType )
{
	HttpResponse response;

	buildProtocol(request,response);
	buildStatusCode(request, response,server, routeType);
	buildHeader(request, response, );

	return response;
};

void HttpResponseBuilder::buildProtocol(const HttpRequest& request,HttpResponse& response)
{
	response.setProtocol(request.getProtocol());
};
void HttpResponseBuilder::buildStatusCode(const HttpRequest& request, HttpResponse& response, const Server& server, RouteType routeType)
{
	if (routeType == RouteType::NOT_FOUND)
	{
		response.setStatus(HttpStatus::NOT_FOUND);
		return;
	}
	
	if (routeType == RouteType::STATIC_FILE)
	{
		if(!std::filesystem::exists(server.getRoot() + request.getTarget()))
		{
			response.setStatus(HttpStatus::NOT_FOUND);	
		}
		else {
			response.setStatus(HttpStatus::OK);
		}
		return;
	}
	
	if (routeType == RouteType::REDIRECT)
	{
		for (const auto& location : server.getLocationBlocks())
			{
				if (request.getTarget().find(location.getPath()) == 0)
				{
					response.setStatus((HttpStatus)location.getRedirectCode().value());
					return;
				}
			}
	}
	if (routeType == RouteType::DELETE_FILE)
	{
		response.setStatus(HttpStatus::NO_CONTENT);
		return ;
	}
	if (routeType == RouteType::CGI)
	{
		response.setStatus(HttpStatus::OK);
		return;
	}
	if (routeType == RouteType::UPLOAD)
	{
		response.setStatus(HttpStatus::CREATED);
		return;
	}
	if (routeType == RouteType::DIRECTORY_LISTING) 
	{
		response.setStatus(HttpStatus::OK);
		return;
	}
}
void HttpResponseBuilder::buildHeader(const HttpRequest& request, HttpResponse& response, const Server& server)
{
	/*Content-Type — what type of content is being sent
		Content-Length — size of the body
		Date — current date/time
		Server — server name (e.g. webserv/1.0)
		Connection — keep-alive or close*/

	response.
}