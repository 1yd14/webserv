/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 15:59:03 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/07/17 13:36:24 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "ConfigParser.hpp"
#include "PathUtils.hpp"
#include "Server.hpp"
#include <cctype>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <iostream>

Config::Config(std::string filename) {
	std::ifstream file(filename);
	std::string configPath = PathUtils::getConfDir(filename);
	if (!file) {
		throw std::runtime_error("config file cannot be opened");
	}
	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	if (content.find('\0') != std::string::npos) {
	    throw std::runtime_error("null byte detected in configuration file");
	}
	ConfigParser::checkBrackets(content);
	std::istringstream stream(content);
	_servers = ConfigParser::parseConfig(stream, configPath);
}

std::vector<Server> Config::getServers() {
	return _servers;
}
