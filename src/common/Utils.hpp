/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 16:29:18 by rmhazres          #+#    #+#             */
/*   Updated: 2026/08/04 12:26:32 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include "../Config/LocationBlock.hpp"
#include "../Config/Server.hpp"
#include "HttpStatus.hpp"



std::string trim(const std::string &str);
int	compareStr(const std::string &str1 , const std::string& str2);
const LocationBlock* findMatchingLocation(const std::string& target, const Server& server);
std::map<std::string, std::string> parseHeaders(const std::string& headerStr);
long safeConvertLong(const std::string& str);
std::string getReasonPhrase(HttpStatus status);
size_t extractContentLength(const std::string& buffer);
std::string unchunkBody(const std::string& chunckedBody, bool &error);
std::string getMimeType(const std::string& path);
std::string urlDecode(const std::string& str);

