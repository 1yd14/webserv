/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 16:30:15 by rmhazres          #+#    #+#             */
/*   Updated: 2026/05/20 16:41:19 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Utils.hpp"
#include <cstddef>


// Removes leading and trailing whitespace (\r, \n, \t, space) from a string
std::string trim(const std::string &str)
{
	size_t start = str.find_first_not_of(" \r\n\t");
	if (start == std::string::npos)
	{
		return "";
	}
	size_t end = str.find_last_not_of(" \r\n\t");

	return str.substr(start, (end- start + 1));
}