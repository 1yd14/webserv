/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_http_pipeline.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 15:12:47 by rmhazres          #+#    #+#             */
/*   Updated: 2026/06/10 16:36:40 by rmhazres         ###   ########.fr       */
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

	std::string raw = "   / HT5TP/1.1\r\nHost: localhost\r\n\r\n";
	
	std::cout << processRequest(raw, servers[0]) << std::endl;
	
    return 0;
}