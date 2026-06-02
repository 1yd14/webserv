/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 16:30:15 by rmhazres          #+#    #+#             */
/*   Updated: 2026/06/02 15:03:36 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Utils.hpp"
#include <cctype>
#include <cstddef>
#include <string>


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

//Compare case-insensative strings
int	compareStr(const std::string& str1 , const std::string& str2 )
{
	if (str1.length() != str2.length())
	{
		return -1;
	}
	for (size_t i = 0; i < str1.length(); i++)
	{
		if(std::tolower(str2[i]) !=  std::tolower(str1[i]))
		{
			return  -1;
		}
	}
	return 0;
}
