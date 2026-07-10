/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpPipeline.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 14:37:07 by rmhazres          #+#    #+#             */
/*   Updated: 2026/07/10 13:23:44 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./HttpPipeline.hpp"
#include "../common/HttpRequest.hpp"
#include "./HttpParser.hpp"
#include "HttpValidator.hpp"
#include "Router.hpp"
#include "HttpResponseBuilder.hpp"

std::string processRequest(const std::string& rawRequest, const Server& server)
{
	HttpRequest request;
	HttpParser parser;
	HttpResponseBuilder builder;
	
	request = parser.parseHttp(rawRequest);
	
	if (request.getStatusCode() != HttpStatus::OK && request.getStatusCode() != HttpStatus::NONE)
	{
		
		HttpResponse response = builder.build(request, server,RouteType::NOT_FOUND );
		return response.serialize();
		return response.serialize();
	}
	HttpValidator validator;
	
	HttpStatus status = validator.validate(request, server);
	
	if (status != HttpStatus::OK)
	{
		HttpResponse response = builder.build(request, server, RouteType::NOT_FOUND );
		return response.serialize();
		return response.serialize();
	}
	Router router;
	const RouteType routeType = router.route(request, server);
	HttpResponse response = builder.build(request, server, routeType);

	return response.serialize();
	return response.serialize();
};
