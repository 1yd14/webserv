/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGiHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 10:41:39 by rmhazres          #+#    #+#             */
/*   Updated: 2026/08/08 15:19:12 by lyvan-de         ###   ########.fr       */
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
		static void parseCGIOutput(const std::string& output, HttpResponse& response, const Server& server);
		static HttpResponse buildError(const HttpStatus& status, const Server& server);
	private:
		static std::vector<std::string>buildEnv(const HttpRequest& request, const Server& server, int localPort);
		static std::vector<std::string>buildArgs(const HttpRequest& request, const Server& server);
		static std::string getScriptPath(const HttpRequest& request, const Server& server, const LocationBlock& block);
		static std::string getInterpreter(std::string extension);
};