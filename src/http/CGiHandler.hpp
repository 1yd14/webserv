/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGiHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 10:41:39 by rmhazres          #+#    #+#             */
/*   Updated: 2026/07/07 17:53:31 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <vector>

#include "../common/HttpRequest.hpp"
#include "../common/HttpResponse.hpp"
#include "../Config/Server.hpp"
#include "../Connection/Connection.hpp"
#include "../Connection/EventLoop.hpp"

class CGIHanlder
{
	public:
		CGIHanlder() = delete;
		
		static void execute(const HttpRequest& request,const Server& server, EventLoop& loop, Connection& connection);
		static void parseCGIOutput(const std::string& output, HttpResponse& response);
		static HttpResponse buildError();
	private:
		static std::vector<std::string>buildEnv(const HttpRequest& request, const Server& server);
		static std::vector<std::string>buildArgs(const HttpRequest& request, const Server& server);
};