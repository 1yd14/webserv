/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationBlock.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 14:44:14 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/05/26 15:11:05 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LocationBlock.hpp"
#include <stdexcept>

std::string	LocationBlock::getPath() {
	return (_path);
}

std::vector<std::string> LocationBlock::getMethods() {
	return (_methods);
}

std::optional<std::string>	LocationBlock::getRoot() {
	return (_root);
}

std::optional<std::string>	LocationBlock::getIndex() {
	return (_index);
}

std::optional<std::string>	LocationBlock::getUploadDir() {
	return (_upload_dir);
}	

std::optional<std::string> LocationBlock::getCgiExtension() {
	return (_cgi_extension);
}

std::optional<bool> LocationBlock::getAutoIndex() {
	return (_autoindex);
}

std::optional<int>	LocationBlock::getRedirectCode() {
	return (_redirect_code);
}

std::optional<std::string>	LocationBlock::getRedirectUrl() {
	return (_redirect_url);
}

void LocationBlock::setPath(std::string path) {
	_path = path;
}

void LocationBlock::setMethods(std::vector<std::string> values) {
    if (values.empty()) {
        throw std::runtime_error("methods directive requires at least one value");
	}
    _methods = values;
}

void LocationBlock::setRoot(std::vector<std::string> values) {
    if (values.size() != 1) {
        throw std::runtime_error("root directive requires exactly one value");
	}
    _root = values[0];
}

void LocationBlock::setIndex(std::vector<std::string> values) {
    if (values.size() != 1) {
        throw std::runtime_error("index directive requires exactly one value");
	}
    _index = values[0];
}

void LocationBlock::setUploadDir(std::vector<std::string> values) {
    if (values.size() != 1) {
        throw std::runtime_error("upload_dir directive requires exactly one value");
	}
    _upload_dir = values[0];
}

void LocationBlock::setCgiExtension(std::vector<std::string> values) {
    if (values.size() != 1) {
        throw std::runtime_error("cgi_extension directive requires exactly one value");
	}
    _cgi_extension = values[0];
}

void LocationBlock::setAutoIndex(std::vector<std::string> values) {
    if (values.size() != 1) {
        throw std::runtime_error("autoindex directive requires exactly one value");
	}
    if (values[0] == "on") {
        _autoindex = true;
	}
    else if (values[0] == "off") {
        _autoindex = false;
	}
    else {
        throw std::runtime_error("autoindex value must be 'on' or 'off'");
	}
}

void LocationBlock::setRedirect(std::vector<std::string> values) {
    if (values.empty() || values.size() > 2) {
        throw std::runtime_error("return directive requires a code and optional url");
	}
    try {
        _redirect_code = std::stoi(values[0]);
    } catch (...) {
        throw std::runtime_error("return directive has invalid status code: " + values[0]);
    }
    if (values.size() == 2) {
        _redirect_url = values[1];
	}
}

