/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 17:03:36 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/07/17 16:38:35 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <csignal>
#include <iostream>
#include "Config/Config.hpp"
#include "Connection/EventLoop.hpp"
#include "server_setup/ListeningSocket.hpp"
#include "Signals/signals.hpp"


int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: ./webserver <config_file>" << std::endl;
        return 1;
    }
	std::vector<Server> servers;
	try {
	    Config conf(argv[1]);
		servers = conf.getServers();
	} catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
		return 1;
	}
	signal(SIGINT, signalHandler);
	signal(SIGTERM, signalHandler);
	signal(SIGPIPE, SIG_IGN);
    EventLoop loop;

    for (size_t i = 0; i < servers.size(); i++) {
        try {
			for (int port : servers[i].getPort()) {
	            loop.addListeningSocket(std::make_unique<ListeningSocket>(servers[i], port));
	            std::cout << "✔ listening on " << servers[i].getHost() << ":" << port << "\n";
			}
        }
        catch (const std::exception& e) {
            std::cerr << "✖ failed to create socket: " << e.what() << "\n";
            return 1;
        }
    }

    std::cout << "Starting event loop...\n";
    loop.run();

    return 0;
}
