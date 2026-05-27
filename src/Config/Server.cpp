/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 13:41:59 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/05/26 14:36:54 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <sstream>

int Server::getPort() {
	return (_port);
}

std::string	Server::getHost() {
	return (_host);
}

std::string Server::getRoot() {
	return (_root);
}

std::string Server::getIndex() {
	return (_index);
}

size_t Server::getMaxBodySize() {
	return (_max_body_size);
}

std::map<int, std::string> Server::getErrorPages() {
	return (_error_pages);
}

std::vector<LocationBlock> Server::getLocationBlocks() {
	return (_location_blocks);
}

void Server::setPort(std::vector<std::string> values) {
	_port = stoi(values[0]);
}

void Server::setHost(std::vector<std::string> values) {
	_host = values[0];
}

void Server::setRoot(std::vector<std::string> values) {
	_root = values[0];
}

void Server::setIndex(std::vector<std::string> values) {
	_index = values[0];
}

void Server::setMaxBodySize(std::vector<std::string> values) {
	std::stringstream sstream(values[0]);
	size_t result;
	sstream >> result;
	if (values[0].back() == 'M') {
		_max_body_size = result * 1024 * 1024;
	}
	else if (values[0].back() == 'K') {
		_max_body_size = result * 1024;
	}
	else {
		_max_body_size = result;
	}
}

void Server::setErrorPages(std::vector<std::string> values) {
	if (values.size() >= 2) {
        _error_pages[std::stoi(values[0])] = values[1];
	}
}

void Server::addLocation(LocationBlock location) {
	_location_blocks.push_back(location);
}


