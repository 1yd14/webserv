/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGiHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 10:41:39 by rmhazres          #+#    #+#             */
/*   Updated: 2026/06/08 13:08:45 by rmhazres         ###   ########.fr       */
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
		static void executeCGI(const std::string& scriptPath, const std::vector<std::string>& env, const std::string& body, HttpResponse& response);
	//	static void parseCGIOutput();
};