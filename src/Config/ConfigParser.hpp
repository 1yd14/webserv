/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 12:59:25 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/07/15 18:00:37 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"


class ConfigParser {
	public:
	static std::vector<Server> parseConfig(std::istringstream& input, const std::string& configDirectory);
	static void checkBrackets(const std::string& config);

	private:
	static Server parseServer(std::istringstream& input, const std::string& configDirectory);
	static LocationBlock	parseLocation(std::istringstream& input, const std::string& configDirectory);
	static std::vector<std::string> readValues(std::istringstream& input);
};

