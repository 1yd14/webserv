/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_config_parser.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 11:52:36 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/07/08 15:48:13 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <iostream>
#include "../src/Config/Config.hpp"
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

        std::vector<LocationBlock> locations = servers[i].getLocationBlocks();
        if (locations.empty()) {
            std::cout << "locations:     (none)\n";
        } else {
            for (size_t j = 0; j < locations.size(); j++) {
                std::cout << "\n  --- Location " << j << " ---\n";
                std::cout << "  path:          " << locations[j].getPath() << "\n";

                std::vector<std::string> methods = locations[j].getMethods();
                if (!methods.empty()) {
                    std::cout << "  methods:       ";
                    for (size_t m = 0; m < methods.size(); m++) {
                        std::cout << methods[m];
                        if (m + 1 < methods.size()) std::cout << ", ";
                    }
                    std::cout << "\n";
                }

                if (locations[j].getRoot().has_value())
                    std::cout << "  root:          " << locations[j].getRoot().value() << "\n";
                if (locations[j].getIndex().has_value())
                    std::cout << "  index:         " << locations[j].getIndex().value() << "\n";
                if (locations[j].getUploadDir().has_value())
                    std::cout << "  upload_dir:    " << locations[j].getUploadDir().value() << "\n";
                if (locations[j].getCgiExtension().has_value())
                    std::cout << "  cgi_extension: " << locations[j].getCgiExtension().value() << "\n";
                if (locations[j].getAutoIndex().has_value())
                    std::cout << "  autoindex:     " << (locations[j].getAutoIndex().value() ? "on" : "off") << "\n";
                if (locations[j].getRedirectCode().has_value()) {
                    std::cout << "  return:        " << locations[j].getRedirectCode().value();
                    if (locations[j].getRedirectUrl().has_value())
                        std::cout << " " << locations[j].getRedirectUrl().value();
                    std::cout << "\n";
                }
            }
        }
        std::cout << "\n";
    }
    return 0;
}