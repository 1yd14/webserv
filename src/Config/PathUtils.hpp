/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PathUtils.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 13:40:04 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/07/09 16:42:45 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

class PathUtils {
	public:
	static std::string getConfDir(std::string& configPath);
	static std::string normalizePath(const std::string& path);
	static std::string joinPath(const std::string& configPath, const std::string& path);
};
