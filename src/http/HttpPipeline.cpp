/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpPipeline.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 14:37:07 by rmhazres          #+#    #+#             */
/*   Updated: 2026/08/10 14:59:22 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./HttpPipeline.hpp"
#include "../common/HttpRequest.hpp"
#include "./HttpParser.hpp"
#include "HttpValidator.hpp"
#include "Router.hpp"
#include "HttpResponseBuilder.hpp"
#include <iostream>

std::string processRequest(HttpRequest& request, const Server& server)
{
	HttpResponseBuilder builder;
	Router router;
	const RouteType routeType = router.route(request, server);
	HttpResponse response = builder.build(request, server, routeType);

	return response.serialize();
};
