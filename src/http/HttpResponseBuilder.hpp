/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponseBuilder.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 16:15:15 by rmhazres          #+#    #+#             */
/*   Updated: 2026/08/08 13:52:21 by lyvan-de         ###   ########.fr       */
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

			static void manageStatic(HttpResponse& response, const std::string& path, const LocationBlock* block, const Server& server);
			static void manageDelete(HttpResponse& response, const LocationBlock& block, const std::string& target, const Server& server);
			static void manageUpload(HttpResponse& response, const HttpRequest& request, const LocationBlock& block, const Server& server);
			static void manageRedirect(HttpResponse& response, const LocationBlock& block, const Server& server);
			static void manageErrorPage(HttpResponse& response, const Server& server);
			static bool parseFormData(HttpResponse& response, const HttpRequest& request, const std::string& uploadDir);
			static void manageDirectory(HttpResponse& response, const HttpRequest& request,const std::string& path, const Server& server);
			static std::string parseFormData(HttpResponse& response, const HttpRequest& request);
			static void sendError(HttpResponse& response, const Server& server, HttpStatus status);
			static std::string resolvePath(const HttpRequest& request, const Server& server, const LocationBlock* block);
};
