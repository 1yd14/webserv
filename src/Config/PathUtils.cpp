/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PathUtils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 13:39:57 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/08/06 15:04:31 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PathUtils.hpp"
#include <sstream>
#include <vector>
#include <iostream>

std::string PathUtils::getConfDir(std::string &configPath) {
	size_t pos = configPath.find_last_of('/');
	if (pos == std::string::npos) {
		return ".";
	}
	if (pos == 0) {
		return "/";
	}
	return configPath.substr(0, pos);
}

std::string PathUtils::normalizePath(const std::string &path) {
	bool isAbsolute = !path.empty() && path[0] == '/';
	std::vector<std::string> segments;
	std::string current;
	std::stringstream pathStream(path);
	std::string result;
	
	while (getline(pathStream, current, '/')) {
		if (current.empty() || current == ".") {
			continue ;
		}
		if (current == "..") {
			if (!segments.empty()) {
				segments.pop_back();
			}
		}
		else {
			segments.push_back(current);
		}
	}
	if (isAbsolute) {
		result = "/";
	}
	for (size_t i = 0; i < segments.size(); i++) {
		result += segments[i];
		if (i + 1 < segments.size()) {
			result += "/";
		}
	}
	if (result.empty()){
		if (isAbsolute) {
			result = "/";
		}
		else {
			result = ".";
		}
	}
	std::cout << "normalizePath input: '" << path << "' output: '" << result << "'\n";
	return result;
}

std::string PathUtils::joinPath(const std::string& configPath, const std::string &path) {
	// std::cout << "joinPath: configPath='" << configPath << "' path='" << path << "'\n";
	if (!path.empty() && path[0] == '/') {
		return (path);
	}
	if (configPath.empty()) {
		return (path);
	}
	if (configPath[configPath.length() - 1] == '/') {
		return configPath + path;
	}
	return normalizePath(configPath + "/" + path);
}
