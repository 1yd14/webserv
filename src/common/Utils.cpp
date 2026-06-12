/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 16:30:15 by rmhazres          #+#    #+#             */
/*   Updated: 2026/06/12 12:36:49 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Utils.hpp"
#include "HttpStatus.hpp"
#include <cctype>
#include <climits>
#include <cstddef>
#include <string>
#include <iostream>


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

const LocationBlock* findMatchingLocation(const std::string& target, const Server& server)
{
	const LocationBlock* best = nullptr;
    size_t bestLen = 0;
    
    for (const auto& location : server.getLocationBlocks())
    {
        const std::string& path = location.getPath();
        if (target.find(path) == 0 && path.length() > bestLen)
        {
            best = &location;
            bestLen = path.length();
        }
    }
    return best;
}

std::map<std::string, std::string> parseHeaders(const std::string& headerStr)
{
	
	std::map<std::string, std::string> header;

	size_t pos = 0;
	size_t delim = 0;
	size_t lineStart = 0;
	while(pos < headerStr.length())
	{
		lineStart = pos;
		pos = headerStr.find("\r\n", lineStart);
		delim = headerStr.find(":", lineStart);
		if (pos == std::string::npos || delim == std::string::npos || delim > pos )
		{
			break;
		}
		std::string key = headerStr.substr(lineStart, delim - lineStart);
		std::string value;
		
		key = trim(key);
		if (delim + 2 < pos)
		{
			value = trim(headerStr.substr(delim + 2, pos - (delim + 2)));
		}
		for( auto &cha : key)
		{
			cha = (char)std::tolower(cha);	
		}
		header.insert({key, value});
		pos+=2;
	}
	return header;
}

long safeConvertLong(const std::string& str)
{
	const int msize = 19;
	if (str.empty())
	{
		return 0;
	}
	if (str.length() == msize && str > std::to_string(LONG_MAX))
	{
		return -1;
	}
	for (char cha : str)
	{
		if(!(bool)isdigit(cha))
		{
			return -1;
		}
	}
	 return std::stol(str);
}
std::string getReasonPhrase(HttpStatus status)
{
	switch (status)
	{
		case HttpStatus::NONE:
		{
			return "NONE";
		}
		case HttpStatus::CONTINUE:
		{
			return "Continue";
		}
		case HttpStatus::OK:
		{
			return "OK";
		}
		case HttpStatus::CREATED:
		{
			return "Created";
		}
		case HttpStatus::NO_CONTENT:
		{
			return "No Content";
		}
		case HttpStatus::MOVED_PERMANETLY:
		{
			return "Moved Permanetly";
		}
		case HttpStatus::FOUND:
		{
			return "Found";
		}
		case HttpStatus::BAD_REQUEST:
		{
			return "Bad Request";
		}
		case HttpStatus::NOT_FOUND:
		{
			return "Not Found";
		}
		case HttpStatus::GONE:
		{
			return "Gone";
		}
		case HttpStatus::LENGTH_REQUIRED:
		{
			return "Length Required";
		}
		case HttpStatus::PAYLOAD_TOO_LARGE:
		{
			return "Payload Too Large";
		}
		case HttpStatus::METHOD_NOT_ALLOWED:
		{
			return "Method Not Allowed";
		}
		case HttpStatus::INTERNAL_SERVER_ERROR:
		{
			return "Internal Server Error";
		}
		case HttpStatus::NOT_IMPLEMENTED:
		{
			return "Not Implemented";
		}
		case HttpStatus::HTTP_VERSION_NOT_SUPPOERTED:
		{
			return "HTTP Version Not Supported";
		}
		default:
			return"Unknown";
	}
}