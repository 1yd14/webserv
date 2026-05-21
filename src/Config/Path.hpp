/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RouteConfig.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 13:34:12 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/05/21 14:37:20 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <string>


class Path {
	public:
	std::string	getPath();
	std::vector<std::string> getMethods();
	std::string	getRoot();
	std::string	getIndex();
	std::string	getUploadDir();	
	std::string getCgiExtension();
	bool getAutoIndex();
	int	getRedirectCode();
	std::string	getRedirectUrl();
	
	private:
	std::string					_path;
	std::vector<std::string>	_methods;
	std::string					_root;
	std::string					_index;
	std::string					_upload_dir;
	std::string					_cgi_extension;
	bool						_autoindex;
	int							_redirect_code;
	std::string					_redirect_url;
};