/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 16:30:15 by rmhazres          #+#    #+#             */
/*   Updated: 2026/08/10 17:29:12 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Utils.hpp"
#include "HttpStatus.hpp"
#include <algorithm>
#include <cctype>
#include <climits>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <fstream>


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
    
    for (const auto& location : server.getLocationBlocks())
    {
        const std::string& path = location.getPath();
        if (target.compare(0, path.size(), path) == 0 &&
    	(path =="/" || target.size() == path.size() || target[path.size()] == '/'))
        {
            best = &location;
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
		if (lineStart < headerStr.length() && (headerStr[lineStart] == ' ' || headerStr[lineStart] == '\t'))
		{
			header["FOLDED_HEADER"] = "true";
			break;
		}
		pos = headerStr.find("\r\n", lineStart);
		delim = headerStr.find_first_of(":", lineStart);
		if(pos == std::string::npos)
		{
			pos = headerStr.length();
		}
		if (delim == std::string::npos || delim > pos )
		{
			break;
		}
		std::string key = headerStr.substr(lineStart, delim - lineStart);
		
		std::string value;
		
		if (delim + 2 < pos)
		{
			value = trim(headerStr.substr(delim + 1, pos - (delim + 1)));
		}
		for( auto &cha : key)
		{
			cha = (char)std::tolower(cha);	
		}
		if (header.contains(key) && header[key] != value)
		{
			header[key] = "DUPLICATE_CONFLICT";
		}
		else {
			header.insert({key, value});
		}
		pos+=2;
	}
	return header;
}

long safeConvertLong(const std::string& str)
{
	if(str.length() > 20)
	{
		return -1;
	}
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
		case HttpStatus::MOVED_PERMANENTLY:
		{
			return "Moved Permanently";
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
		case HttpStatus::FORBIDDEN:
		{
			return "Forbidden";
		}
		case HttpStatus::URI_TOO_LONG:
		{
			return "URI Too Long";
		}
		case HttpStatus::REQUEST_HEADER_LARGE:
		{
			return "Request Header Fields Too Large";
		}
		default:
			return"Unknown";
	}
}

size_t extractContentLength(const std::string& buffer)
{
	std::string lowerBuffer = buffer;

	std::transform(
		lowerBuffer.begin(),
		lowerBuffer.end(),
		lowerBuffer.begin(),
		[](unsigned char c) {
		    return std::tolower(c);
		}
	);
	size_t pos = lowerBuffer.find("content-length:");
	long val = 0;
	if (pos != std::string::npos)
	{
		std::string sub = buffer.substr( pos +15);
		auto itt =  sub.find_first_of("\r\n");

		val = safeConvertLong(trim(sub.substr(0,itt)));
		if(val < 0)
		{
			return 0;
		}
	}
	return  (size_t)val;
}

std::string unchunkBody(const std::string &chunckedBody, bool &error)
{
	std::string result;
	std::string line;
	size_t end;

	size_t position = 0;
	while (position != std::string::npos) 
	{
		end = chunckedBody.find("\r\n", position);
		if (end == std::string::npos)
		{
			return result;
		}
		line = chunckedBody.substr(position, end -position);
		try {
			size_t chunckSize = std::stoul(line, nullptr, 16);		
			if (chunckSize == 0)
			{
				return result;
			}
			result += chunckedBody.substr(end +2, chunckSize);
			position = end + 2 + chunckSize + 2;
		} catch (const std::exception& e) {
		
			error = true;
			return "";
		}
	}
	return result;
}

std::string getMimeType(const std::string& path)
{
	size_t pos = path.find_last_of(".");
	if (pos == std::string::npos)
	{
		return "application/octet-stream";
	}
	std::string ext = path.substr(pos);
	
	std::map<std::string, std::string> mimeTypes = {
		{".html", "text/html"},
        {".css", "text/css"},
        {".js", "application/javascript"},
        {".jpg", "image/jpeg"},
        {".jpeg", "image/jpeg"},
        {".png", "image/png"},
        {".gif", "image/gif"},
        {".txt", "text/plain"},
        {".pdf", "application/pdf"},
        {".ico", "image/x-icon"},
        {".py", "text/plain"}
	};
	auto it = mimeTypes.find(ext);
	if (it != mimeTypes.end())
	{
		return it->second;
	}
	return "application/octet-stream";
}
std::string urlDecode(const std::string& str)
{
    std::string result;
    for (size_t i = 0; i < str.length(); i++)
    {
        if (str[i] == '%' && i + 2 < str.length())
        {
            std::string hex = str.substr(i + 1, 2);
            char decoded = (char)std::stoul(hex, nullptr, 16);
            result += decoded;
            i += 2;
        }
        else if (str[i] == '+')
		{
            result += ' ';
		}
        else
		{
            result += str[i];
		}
    }
    return result;
}

void getErrorBody(HttpResponse& response, const Server& server) {
	int status = (int)response.getStatus();
	const auto& errorPages = server.getErrorPages();

	auto itt = errorPages.find(status);
	if (itt != errorPages.end())
	{
		std::ifstream file(itt->second);
		if(!file.is_open())
		{
            response.setBody("<html><body><h1>" + std::to_string((int)response.getStatus()) + " Error</h1></body></html>");
			return;
		}
		std::string body((std::istreambuf_iterator<char>(file)),
					 	std::istreambuf_iterator<char>());
		response.setBody(body);
		return;
	}
	response.setBody("<html><body><h1>" + std::to_string((int)response.getStatus()) + " Error</h1></body></html>");
}
