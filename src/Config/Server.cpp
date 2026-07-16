/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 13:41:59 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/07/16 15:26:05 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "LocationBlock.hpp"
#include "PathUtils.hpp"
#include <sstream>
#include <stdexcept>

#include <iostream>

Server::Server() 
    : _port(0),
      _max_body_size(0),
      _finalized(false) {}

int Server::getPort() const {
	return (_port);
}

std::string	Server::getHost() const {
	return (_host);
}

std::string Server::getRoot() const {
	return (_root);
}

std::string Server::getIndex() const {
	return (_index);
}

size_t Server::getMaxBodySize() const {
	return (_max_body_size);
}

const std::map<int, std::string>& Server::getErrorPages() const {
	return (_error_pages);
}

const std::vector<LocationBlock>& Server::getLocationBlocks() const{
	return (_location_blocks);
}

void Server::setConfigDirectory(std::string configPath) {
	_configDirectory = configPath;
}

void Server::setPort(std::vector<std::string> values) {
	if (values.size() != 1) {
		throw std::runtime_error("listen directive requires only one value");
	}
	try {
		_port = stoi(values[0]);
	}
	catch (...) {
		throw std::runtime_error("listen: invalid port number: " + values[0]);
	}
	if (_port <= 0 || _port >= 65535) {
		throw std::runtime_error("listen: port number out of range: " + values[0]);
	}
}

void Server::setHost(std::vector<std::string> values) {
	if (values.size() != 1) {
		throw std::runtime_error("host directive requires at least one value");
	}
	if (values[0] == "localhost") {
		_host = "127.0.0.1";
	} else {
		_host = values[0];
	}
}

void Server::setRoot(std::vector<std::string> values) {
	if (values.size() != 1) {
		throw std::runtime_error("root directive requires at least one value");
	}
	_root = PathUtils::normalizePath(PathUtils::joinPath(_configDirectory, values[0]));
}

void Server::setIndex(std::vector<std::string> values) {
	if (values.size() != 1) {
		throw std::runtime_error("index directive requires at least one value");
	}
	_index = values[0];
}

void Server::setMaxBodySize(std::vector<std::string> values) {
	if (values.size() != 1) {
		throw std::runtime_error("max_body_size directive requires exactly one value");
	}
	const std::string& val = values[0];
	if (values[0][0] == '-') {
		throw std::runtime_error("max_body_size cannot be negative");
	}
	char suffix = val.back();
	std::string numberPart = (suffix == 'M' || suffix == 'K') ? val.substr(0, val.size() - 1) : val;

	size_t result;
	std::istringstream sstream(numberPart);
	if (!(sstream >> result) || !sstream.eof()) {
		throw std::runtime_error("max_body_size: invalid value: " + val);
	}
	if (suffix == 'M') {
		_max_body_size = result * 1024 * 1024;
	}
	else if (suffix == 'K') {
		_max_body_size = result * 1024;
	}
	else {
		_max_body_size = result;
	}
}

void Server::setErrorPages(std::vector<std::string> values) {
	if (values.size() != 2) {
		throw std::runtime_error("error_page directive requires exactly two values (code path)");
	}
	int code;
	try {
		code = std::stoi(values[0]);
	} catch (...) {
		throw std::runtime_error("error_page: invalid status code: " + values[0]);
	}
	if (code < 400 || code > 599) {
		throw std::runtime_error("error_page: code must be 4xx or 5xx, got: " + values[0]);
	}
	_error_pages[code] = PathUtils::normalizePath(PathUtils::joinPath(_configDirectory, values[1]));
}

void Server::addLocation(const LocationBlock& location) {
	_location_blocks.push_back(location);
}

void Server::finalize() {
	if (_finalized) {
		return ;
	}
	if (_host.empty()) {
		_host = "0.0.0.0";
	}
	if (_port == 0) {
		throw std::runtime_error("server: missing port");
	}
	if (_root.empty()) {
		throw std::runtime_error("server: missing root");
	}
	//if (_location_blocks.empty()) {
	//	throw std::runtime_error("server: no locations defined");
	//}
	for (size_t i = 0; i < _location_blocks.size(); i ++) {
		LocationBlock &loc = _location_blocks[i];
		if (! loc.getRoot()) {
			loc.setResolvedRoot(_root);
		}
		if (! loc.getIndex()) {
			loc.setResolvedIndex(_index);
		}
		if (! loc.getAutoIndex()) {
			loc.setResolvedAutoIndex(false);
		}
	}
	//checks duplicates
	for (size_t i = 0; i < _location_blocks.size(); i++) {
		for (size_t j = i + 1; j < _location_blocks.size(); j++) {
			if (_location_blocks[i].getPath() == _location_blocks[j].getPath()) {
				throw std::runtime_error("duplicate location path: " +
										_location_blocks[i].getPath());
			}
		}
	}
	_finalized = true;
}
