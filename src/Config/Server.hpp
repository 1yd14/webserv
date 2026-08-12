/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 12:34:17 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/08/12 11:22:24 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "LocationBlock.hpp"
#include <map>
#include <string>
#include <vector>


class Server {
	public:
	Server();
	[[nodiscard]] std::vector<int> getPort() const;
	[[nodiscard]] std::string	getHost() const;
	[[nodiscard]] std::string getRoot() const;
	[[nodiscard]] std::string getIndex() const;
	[[nodiscard]] long long getMaxBodySize() const;
	[[nodiscard]] const std::map<int, std::string>& getErrorPages() const;
	[[nodiscard]] const std::vector<LocationBlock>& getLocationBlocks() const;
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
	long long					_max_body_size;
	std::map<int, std::string>	_error_pages;
	std::vector<LocationBlock>	_location_blocks;
	bool						_finalized;
};

