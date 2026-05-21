/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 11:52:36 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/05/27 14:47:39 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Config/Config.hpp"


int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: ./webserver <config_file>" << std::endl;
        return 1;
    }

    Config conf(argv[1]);
    std::vector<Server> servers = conf.getServers();

    std::cout << "Parsed " << servers.size() << " server(s)\n\n";

    for (size_t i = 0; i < servers.size(); i++) {
        std::cout << "--- Server " << i << " ---\n";
        std::cout << "port:          " << servers[i].getPort() << "\n";
        std::cout << "host:          " << servers[i].getHost() << "\n";
        std::cout << "root:          " << servers[i].getRoot() << "\n";
        std::cout << "index:         " << servers[i].getIndex() << "\n";
        std::cout << "max_body_size: " << servers[i].getMaxBodySize() << "\n";

        std::map<int, std::string> errorPages = servers[i].getErrorPages();
        std::map<int, std::string>::const_iterator it;
        for (it = errorPages.begin(); it != errorPages.end(); ++it)
            std::cout << "error_page:    " << it->first << " -> " << it->second << "\n";
        std::cout << "\n";
    }
    return 0;
}
