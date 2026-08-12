/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationBlock.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 13:34:12 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/08/12 11:03:25 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <string>
#include <optional>

class LocationBlock {
	public:
	LocationBlock();
	[[nodiscard]] std::string	getPath() const ;
	[[nodiscard]] std::vector<std::string> getMethods() const ;
	[[nodiscard]] std::optional<std::string>	getRoot() const ;
	[[nodiscard]] std::optional<std::string>	getIndex() const ;
	[[nodiscard]] std::optional<std::string>	getUploadDir() const ;	
	[[nodiscard]] std::optional<std::string> getCgiExtension() const ;
	[[nodiscard]] std::optional<bool> getAutoIndex() const ;
	[[nodiscard]] std::optional<int>	getRedirectCode() const ;
	[[nodiscard]] std::optional<std::string>	getRedirectUrl() const ;
	[[nodiscard]] std::optional<long long> getMaxBodySize() const ;
	void setConfigDirectory(std::string configPath);
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
	void setMaxBodySize(std::vector<std::string> values);
	void validate();
	void freeze();

	private:
	std::string					_configDirectory;
	std::string					_path;
	std::vector<std::string>	_methods;
	std::optional<std::string>	_root;
	std::optional<std::string>	_index;
	std::optional<std::string>	_upload_dir;
	std::optional<std::string>	_cgi_extension;
	std::optional<bool>			_autoindex;
	std::optional<int>			_redirect_code;
	std::optional<std::string>	_redirect_url;
	std::optional<long long>	_max_body_size;
	bool						_finalized = false;
};