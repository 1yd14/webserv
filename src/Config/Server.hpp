/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 12:34:17 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/05/21 15:40:07 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstddef>
#include <map>
#include <vector>
#include <string>
#include "Path.hpp"


class Server {
	public:
	int getPort();
	std::string	getHost();
	std::string getRoot();
	std::string getIndex();
	size_t getMaxBodySize();
	std::map<int, std::string> getErrorPages();
	std::vector<Path> getPaths();
	void setPort(std::vector<std::string> values);
	void setHost(std::vector<std::string> values);
	void setRoot(std::vector<std::string> values);
	void setIndex(std::vector<std::string> values);
	void setMaxBodySize(std::vector<std::string> values);
	void setErrorPages(std::vector<std::string> values);


	private:
	int							_port;
	std::string					_host;
	std::string					_root;
	std::string					_index;
	size_t						_max_body_size;
	std::map<int, std::string>	_error_pages;
	std::vector<Path>			_paths;
};

