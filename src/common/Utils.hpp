/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 16:29:18 by rmhazres          #+#    #+#             */
/*   Updated: 2026/06/08 13:21:59 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include "../Config/LocationBlock.hpp"
#include "../Config/Server.hpp"




std::string trim(const std::string &str);
int	compareStr(const std::string &str1 , const std::string& str2);
const LocationBlock* findMatchingLocation(const std::string& target, const Server& server);
