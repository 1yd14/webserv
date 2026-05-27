/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 12:59:25 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/05/21 16:49:46 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"


class ConfigParser {
	public:
	static std::vector<Server> parseConfig(std::istringstream& input);

	private:
	static Server parseServer(std::istringstream& input);
	static LocationBlock	parseLocation(std::istringstream& input);
	static std::vector<std::string> readValues(std::istringstream& input);
};

