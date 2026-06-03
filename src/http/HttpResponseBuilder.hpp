/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponseBuilder.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 16:15:15 by rmhazres          #+#    #+#             */
/*   Updated: 2026/06/03 16:30:56 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../common/HttpResponse.hpp"
#include "../common/HttpRequest.hpp"
#include "../common/RouteType.hpp"
#include "../Config/Server.hpp"

class HttpResponseBuilder
{
	public:
			HttpResponseBuilder();
			~HttpResponseBuilder();
			HttpResponseBuilder(const HttpResponseBuilder &other) = delete;
			HttpResponseBuilder& operator=(const HttpResponseBuilder &other) = delete;			
			HttpResponse build(const HttpRequest &request,const Server &server, RouteType routeType);
	private:
			static void buildProtocol(const HttpRequest& request, HttpResponse& response);
			static void buildStatusCode(const HttpRequest& request, HttpResponse& response,const Server& server, RouteType routeType);
			static void buildHeader(const HttpRequest& request, HttpResponse& response, const Server& server);
};