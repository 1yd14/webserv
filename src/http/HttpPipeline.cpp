/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpPipeline.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 14:37:07 by rmhazres          #+#    #+#             */
/*   Updated: 2026/06/09 17:03:42 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./HttpPipeline.hpp"
#include "../common/HttpRequest.hpp"
#include "./HttpParser.hpp"
#include "CGiHandler.hpp"
#include "HttpValidator.hpp"
#include "Router.hpp"
#include "HttpResponseBuilder.hpp"

#include <iostream>

std::string processRequest(const std::string& rawRequest, const Server& server)
{
	HttpRequest request;

	HttpParser parser;

	request = parser.parseHttp(rawRequest);
	HttpValidator validator;
	
	HttpStatus status = validator.validate(request, server.getMaxBodySize());

	if (status != HttpStatus::OK && status != HttpStatus::CONTINUE)
	{
		// do i need to do something here ?
	}
	Router router;
	const RouteType routeType = router.route(request, server);
	
	HttpResponseBuilder builder;
	HttpResponse response = builder.build(request, server, routeType);

	CGIHanlder::execute(request, server, response);
	
	return response.serlialize();
};
