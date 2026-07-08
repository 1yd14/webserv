/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PathUtils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 13:39:57 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/07/08 17:53:26 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PathUtils.hpp"

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

//std::string PathUtils::normalizePath(std::string &path) {
	
//}
