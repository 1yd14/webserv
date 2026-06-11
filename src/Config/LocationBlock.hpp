/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationBlock.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 13:34:12 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/06/04 14:58:51 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <string>
#include <optional>

class LocationBlock {
	public:
	std::string	getPath() const ;
	std::vector<std::string> getMethods() const ;
	std::optional<std::string>	getRoot() const ;
	std::optional<std::string>	getIndex() const ;
	std::optional<std::string>	getUploadDir() const ;	
	std::optional<std::string> getCgiExtension() const ;
	std::optional<bool> getAutoIndex() const ;
	std::optional<int>	getRedirectCode() const ;
	std::optional<std::string>	getRedirectUrl() const ;
	void setPath(std::string path);
	void setMethods(std::vector<std::string> value);
	void setRoot(std::vector<std::string> value);
	void setResolvedRoot(std::string root);
	void setIndex(std::vector<std::string> value);
	void setResolvedIndex(std::string index);
	void setUploadDir(std::vector<std::string> value);
	void setCgiExtension(std::vector<std::string> value);
	void setAutoIndex(std::vector<std::string> value);
	void setResolvedAutoIndex(bool value);
	void setRedirect(std::vector<std::string> value);
	void validate();
	void freeze();

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