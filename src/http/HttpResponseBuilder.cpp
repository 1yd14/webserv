/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponseBuilder.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 16:15:48 by rmhazres          #+#    #+#             */
/*   Updated: 2026/06/03 12:26:39 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponseBuilder.hpp"


HttpResponseBuilder::HttpResponseBuilder() = default;
HttpResponseBuilder::~HttpResponseBuilder(){};

HttpResponse HttpResponseBuilder::build(HttpRequest const &request, Server const &server)
{
	HttpResponse response;

	buildProtocol(request,response);
	buildStatusCode(request, response,server);
};

void buildProtocol(const HttpRequest& request,HttpResponse& response)
{
	response.setProtocol(request.getProtocol());
};
void buildStatusCode(const HttpRequest& request, HttpResponse& response, const Server& server)
{
	
}