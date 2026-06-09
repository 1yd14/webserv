/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_http_pipeline.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 15:12:47 by rmhazres          #+#    #+#             */
/*   Updated: 2026/06/09 17:59:34 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <iostream>
#include "../src/Config/Config.hpp"
#include "../src/server_setup/ListeningSocket.hpp"
#include "../src/http/HttpPipeline.hpp"
#include <string>
#include <vector>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: ./webserver <config_file>" << std::endl;
        return 1;
    }
	
    Config conf(argv[1]);
    std::vector<Server> servers = conf.getServers();

	std::string raw = "POST /index.html HTTP/1.1\r\nHost: bla \r\nContent-Length: 10\r\n\r\nhelloworld";

	// const auto& locationBlocks = servers[0].getLocationBlocks();

	// const std::string& target = "/upload";
	// for (const auto& location : locationBlocks)
	// {
	// 	if (target == location.getPath())
	// 	{
	// 		std::cout  <<"yessss\n";
	// 		std::cout << "locations " << location.getPath() << std::endl;
	// 	}
	// }
	
	std::cout << processRequest(raw, servers[0]) << std::endl;
	
    return 0;
}