/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 12:34:17 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/05/07 12:57:00 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_CONFIG_HPP
# define SERVER_CONFIG_HPP

#include <cstddef>
#include <map>
#include <vector>
#include <string>

struct RouteConfig {
	std::string					path;
	std::vector<std::string>	methods;
	std::string					root;
	std::string					index;
	std::string					upload_dir;
	std::string					cgi_extension;
	bool						autoindex;
	int							redirect_code;
	std::string					redirect_url;
};

struct ServerConfig {
	std::string					host;
	int							port;
	std::string					root;
	std::string					index;
	size_t						max_body_size;
	std::map<int, std::string>	error_pages;
	std::vector<RouteConfig>	routes;
};


#endif
