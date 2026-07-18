/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 12:34:17 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/07/17 16:25:55 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstddef>
#include <map>
#include <vector>
#include <string>
#include "LocationBlock.hpp"


class Server {
	public:
	Server();
	std::vector<int> getPort() const;
	std::string	getHost() const;
	std::string getRoot() const;
	std::string getIndex() const;
	size_t getMaxBodySize() const;
	const std::map<int, std::string>& getErrorPages() const;
	const std::vector<LocationBlock>& getLocationBlocks() const;
	void setConfigDirectory(std::string configPath);
	void setPort(std::vector<std::string> values);
	void setHost(std::vector<std::string> values);
	void setRoot(std::vector<std::string> values);
	void setIndex(std::vector<std::string> values);
	void setMaxBodySize(std::vector<std::string> values);
	void setErrorPages(std::vector<std::string> values);
	void addLocation(const LocationBlock& location);
	void finalize();

	private:
	std::string					_configDirectory;
	std::vector<int>			_port;
	std::string					_host;
	std::string					_root;
	std::string					_index;
	size_t						_max_body_size;
	std::map<int, std::string>	_error_pages;
	std::vector<LocationBlock>	_location_blocks;
	bool						_finalized;
};

