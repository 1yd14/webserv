/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGiHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 10:41:39 by rmhazres          #+#    #+#             */
/*   Updated: 2026/06/08 16:05:09 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../common/HttpRequest.hpp"
#include "../common/HttpResponse.hpp"
#include "../Config/Server.hpp"
#include <string>
#include <vector>

class CGIHanlder
{
	public:
		CGIHanlder() = delete;
		
		static void execute(const HttpRequest& request, const Server& server, HttpResponse& response);
	private:
		static std::vector<std::string>buildEnv(const HttpRequest& request, const Server& server);
		static void executeCGI(const std::vector<char*>& argv, const std::vector<std::string>& env, HttpResponse& response,const std::string& body);
		static void parseCGIOutput(const std::string& output, HttpResponse& response);
};