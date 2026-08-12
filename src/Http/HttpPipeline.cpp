/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpPipeline.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 14:37:07 by rmhazres          #+#    #+#             */
/*   Updated: 2026/08/12 12:36:31 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./HttpPipeline.hpp"
#include "../Common/HttpRequest.hpp"
#include "./HttpParser.hpp"
#include "Router.hpp"
#include "HttpResponseBuilder.hpp"

std::string processRequest(const HttpRequest& request, const Server& server)
{
	HttpResponseBuilder builder;
	Router router;
	const RouteType routeType = router.route(request, server);
	HttpResponse response = builder.build(request, server, routeType);

	return response.serialize();
};
