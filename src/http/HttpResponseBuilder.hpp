/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponseBuilder.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 16:15:15 by rmhazres          #+#    #+#             */
/*   Updated: 2026/06/12 15:36:03 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../common/HttpResponse.hpp"
#include "../common/HttpRequest.hpp"
#include "../common/RouteType.hpp"
#include "../Config/Server.hpp"
#include <string>

class HttpResponseBuilder
{
	public:
			HttpResponseBuilder();
			~HttpResponseBuilder();
			HttpResponseBuilder(const HttpResponseBuilder &other) = delete;
			HttpResponseBuilder& operator=(const HttpResponseBuilder &other) = delete;			
			HttpResponse build(const HttpRequest &request,const Server &server, RouteType routeType);
	private:
			static void buildStatusCode(const HttpRequest& request, HttpResponse& response,const Server& server, RouteType routeType);
			static void buildHeader(const HttpRequest& request, HttpResponse& response);
			static void buildBody(const HttpRequest& request,HttpResponse& response,const Server& server,RouteType routeType, const LocationBlock* block);

			static void manageStatic(HttpResponse& response, const std::string& path);
			static void manageDelete(HttpResponse& response, const LocationBlock& block, const std::string& target);
			static void manageUpload(HttpResponse& response, const HttpRequest& request, const LocationBlock& block);
			static void manageRedirect(HttpResponse& response, const LocationBlock& block);
			static void manageErrorPage(HttpResponse& response, const Server& server);
			static void manageDirectory(HttpResponse& response, const HttpRequest& request,const std::string& path);
};