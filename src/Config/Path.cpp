/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Path.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 14:44:14 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/05/21 14:45:52 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Path.hpp"

std::string	Path::getPath() {
	return (_path);
}

std::vector<std::string> Path::getMethods() {
	return (_methods);
}

std::string	Path::getRoot() {
	return (_root);
}

std::string	Path::getIndex() {
	return (_index);
}

std::string	Path::getUploadDir() {
	return (_upload_dir);
}	

std::string Path::getCgiExtension() {
	return (_cgi_extension);
}

bool Path::getAutoIndex() {
	return (_autoindex);
}

int	Path::getRedirectCode() {
	return (_redirect_code);
}

std::string	Path::getRedirectUrl() {
	return (_redirect_url);
}
