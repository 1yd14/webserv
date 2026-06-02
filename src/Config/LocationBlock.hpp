/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationBlock.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 13:34:12 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/06/02 16:16:28 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <string>
#include <optional>

class LocationBlock {
	public:
	std::string	getPath();
	std::vector<std::string> getMethods();
	std::optional<std::string>	getRoot();
	std::optional<std::string>	getIndex();
	std::optional<std::string>	getUploadDir();	
	std::optional<std::string> getCgiExtension();
	std::optional<bool> getAutoIndex();
	std::optional<int>	getRedirectCode();
	std::optional<std::string>	getRedirectUrl();
	void setPath(std::string path);
	void setMethods(std::vector<std::string> value);
	void setRoot(std::vector<std::string> value);
	void setIndex(std::vector<std::string> value);
	void setUploadDir(std::vector<std::string> value);
	void setCgiExtension(std::vector<std::string> value);
	void setAutoIndex(std::vector<std::string> value);
	void setRedirect(std::vector<std::string> value);
	void finalize();

	private:
	std::string					_path;
	std::vector<std::string>	_methods;
	std::optional<std::string>	_root;
	std::optional<std::string>	_index;
	std::optional<std::string>	_upload_dir;
	std::optional<std::string>	_cgi_extension;
	std::optional<bool>			_autoindex;
	std::optional<int>			_redirect_code;
	std::optional<std::string>	_redirect_url;
	bool						_finalized = false;
};