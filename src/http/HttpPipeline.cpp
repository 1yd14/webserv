/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpPipeline.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 14:37:07 by rmhazres          #+#    #+#             */
/*   Updated: 2026/08/10 11:07:58 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./HttpPipeline.hpp"
#include "../common/HttpRequest.hpp"
#include "./HttpParser.hpp"
#include "HttpValidator.hpp"
#include "Router.hpp"
#include "HttpResponseBuilder.hpp"
#include <iostream>

std::string processRequest(const HttpRequest& request, const Server& server)
{
	HttpResponseBuilder builder;
	
	if (request.getStatusCode() != HttpStatus::OK && request.getStatusCode() != HttpStatus::NONE)
	{
		HttpResponse response = builder.build(request, server,RouteType::NOT_FOUND );
		response.setHeader("Connection", "close");
		return response.serialize();
	}
	HttpValidator validator;
	
	HttpStatus status = validator.validate(request, server);
	
	if (status != HttpStatus::OK)
	{
		HttpResponse response = builder.build(request, server, RouteType::NOT_FOUND);
		response.setHeader("Connection", "close");
		return response.serialize();
	}
	Router router;
	const RouteType routeType = router.route(request, server);
	HttpResponse response = builder.build(request, server, routeType);

	return response.serialize();
};
