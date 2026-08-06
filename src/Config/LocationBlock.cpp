/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationBlock.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 14:44:14 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/08/06 15:11:21 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LocationBlock.hpp"
#include <algorithm>
#include <stdexcept>
#include <set>
#include "PathUtils.hpp"

LocationBlock::LocationBlock()
:     _root(std::nullopt),
	  _index(std::nullopt),
	  _upload_dir(std::nullopt),
	  _cgi_extension(std::nullopt),
      _autoindex(std::nullopt),
      _redirect_code(std::nullopt),
      _redirect_url(std::nullopt),
      _finalized(false)
{
}

std::string	LocationBlock::getPath() const {
	return (_path);
}

std::vector<std::string> LocationBlock::getMethods() const {
	return (_methods);
}

std::optional<std::string>	LocationBlock::getRoot() const {
	return (_root);
}

std::optional<std::string>	LocationBlock::getIndex() const {
	return (_index);
}

std::optional<std::string>	LocationBlock::getUploadDir() const {
	return (_upload_dir);
}	

std::optional<std::string> LocationBlock::getCgiExtension() const {
	return (_cgi_extension);
}

std::optional<bool> LocationBlock::getAutoIndex() const {
	return (_autoindex);
}

std::optional<int>	LocationBlock::getRedirectCode() const {
	return (_redirect_code);
}

std::optional<std::string>	LocationBlock::getRedirectUrl() const {
	return (_redirect_url);
}

void LocationBlock::setConfigDirectory(std::string configPath) {
	_configDirectory = configPath;
}

void LocationBlock::setPath(std::string path) {
	if (_finalized) {
		return ;
	}
	if (path.empty() || path[0] != '/') {
	    throw std::runtime_error("location path must start with '/'");
	}
	_path = path;
}

void LocationBlock::setMethods(std::vector<std::string> values) {
	if (_finalized) {
		return ;
	}
	if (values.size() < 1) {
		throw std::runtime_error("methods directive requires at least one value");
	}
    static const std::set<std::string> allowed = {
        "GET",
        "POST",
        "DELETE"
    };

    for (std::vector<std::string>::const_iterator it = values.begin();
         it != values.end(); ++it) {

        if (allowed.find(*it) == allowed.end()) {
            throw std::runtime_error("Invalid HTTP method: " + *it);
        }
    }
	_methods = values;
}

void LocationBlock::setRoot(std::vector<std::string> values) {
	if (_finalized) {
		return ;
	}
	if (values.size() != 1) {
		throw std::runtime_error("root directive requires exactly one value");
	}
	if (values[0].empty()) {
	    throw std::runtime_error("root cannot be empty");
	}
	_root = PathUtils::normalizePath(PathUtils::joinPath(_configDirectory, values[0]));
}

void LocationBlock::setResolvedRoot(std::string root) {
	// _root = PathUtils::normalizePath(PathUtils::joinPath(_configDirectory, root));;
	  _root = root;
}

void LocationBlock::setIndex(std::vector<std::string> values) {
	if (_finalized) {
		return ;
	}
	if (values.size() != 1) {
		throw std::runtime_error("index directive requires exactly one value");
	}
	if (values[0].find('/') != std::string::npos) {
	    throw std::runtime_error("index must be a filename, not a path");
	}
	_index = values[0];
}

void LocationBlock::setResolvedIndex(std::string index) {
	_index = index;
}

void LocationBlock::setUploadDir(std::vector<std::string> values) {
	if (_finalized) {
		return ;
	}
	if (values.size() != 1) {
		throw std::runtime_error("upload_dir directive requires exactly one value");
	}
	_upload_dir = PathUtils::normalizePath(PathUtils::joinPath(_configDirectory, values[0]));
}

void LocationBlock::setCgiExtension(std::vector<std::string> values) {
	if (_finalized) {
		return ;
	}
	if (values.size() != 1) {
		throw std::runtime_error("cgi_extension directive requires exactly one value");
	}
	if (values[0].empty() || values[0][0] != '.') {
	    throw std::runtime_error("cgi_extension must start with '.'");
	}
	_cgi_extension = values[0];
}

void LocationBlock::setAutoIndex(std::vector<std::string> values) {
	if (_finalized) {
		return ;
	}
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

void LocationBlock::setResolvedAutoIndex(bool value) {
	_autoindex = value;
}

void LocationBlock::setRedirect(std::vector<std::string> values) {
	if (_finalized) {
		return ;
	}
	if (values.empty() || values.size() > 2) {
		throw std::runtime_error("return directive requires a code and optional url");
	}
	try {
		_redirect_code = std::stoi(values[0]);
	} catch (...) {
		throw std::runtime_error("return directive has invalid status code: " + values[0]);
	}
	if (_redirect_code != 301 && _redirect_code != 302 &&
    		_redirect_code != 303 && _redirect_code != 307 &&
    		_redirect_code != 308) {
    	throw std::runtime_error("invalid redirect code");
	}
	if (values.size() == 2) {
		_redirect_url = values[1];
	}
}

void LocationBlock::validate() {
	if (_finalized) {
		return ;
	}
	if (_path.empty()) {
		throw std::runtime_error("missing location path");
	}
	if (!_redirect_code &&_methods.empty()) {
		throw std::runtime_error("methods required");
	}
	if (!_autoindex) {
		_autoindex = false;
	}
	//check for cross checks needed
	if (_upload_dir) {
		if (std::ranges::find(_methods, "POST") == _methods.end()) {
			throw std::runtime_error("upload_dir requires POST method");
		}
	}
	if (_redirect_code) {
		if (!_redirect_url) {
			throw std::runtime_error("redirect requires URL");
		}
		if (_autoindex == true || _index.has_value() || _root.has_value() || _upload_dir.has_value()) {
			throw std::runtime_error("invalid redirect block.");
		}
	}
}

void LocationBlock::freeze() {
	_finalized = true;
}
