/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 15:59:03 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/06/04 17:11:25 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "ConfigParser.hpp"
#include "Server.hpp"
#include <fstream>
#include <sstream>
#include <vector>

Config::Config(std::string filename) {
	std::ifstream file(filename);
	if (!file) {
		//handle error
	}
	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	std::istringstream stream(content);
	_servers = ConfigParser::parseConfig(stream);
}

std::vector<Server> Config::getServers() {
	return _servers;
}
