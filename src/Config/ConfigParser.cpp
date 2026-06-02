/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 13:32:34 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/06/02 16:35:30 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"
#include "LocationBlock.hpp"
#include "Server.hpp"
#include <sstream>
#include <stdexcept>
#include <vector>


std::vector<std::string> ConfigParser::readValues(std::istringstream& input) {
    std::vector<std::string> values;
    std::string word;

    while (input >> word) {
        if (word.back() == ';') {
            word.pop_back();
            if (!word.empty()) {
                values.push_back(word);
			}
            break;
        }
        values.push_back(word);
    }
    return values;
}

LocationBlock ConfigParser::parseLocation(std::istringstream &input) {
	LocationBlock result;
	std::string word;

	if (!(input>>word)) {
		throw std::runtime_error("expected location path");
	};
	if (word.back() =='{') {
		word.pop_back();
	}
	else {
		std::string brace;
		if (! (input>>brace) || brace != "{") {
			throw std::runtime_error("Expected '{' after location path");
		}
	}
	result.setPath(word);
	using Setter = void (LocationBlock::*)(std::vector<std::string>);
    const std::map<std::string, Setter> dispatch = {
        { "methods",      &LocationBlock::setMethods      },
        { "root",         &LocationBlock::setRoot         },
        { "index",        &LocationBlock::setIndex        },
        { "upload_dir",   &LocationBlock::setUploadDir    },
        { "cgi_extension",&LocationBlock::setCgiExtension },
        { "autoindex",    &LocationBlock::setAutoIndex    },
        { "redirect",     &LocationBlock::setRedirect     },
    };
	while (input>>word) {
		if (word == "}") {
			break ;
		}
		std::vector<std::string> values = readValues(input);
		std::map<std::string, Setter>::const_iterator iter = dispatch.find(word);
		if (iter != dispatch.end()) {
    		(result.*(iter->second))(values);
		}
		else {
			throw std::runtime_error("Unknown location directive: " + word);
		}
	}
	result.finalize();
	return result;
}

Server ConfigParser::parseServer(std::istringstream& input) {
	Server result;
	std::string word;

    using Setter = void (Server::*)(std::vector<std::string>);
    const std::map<std::string, Setter> dispatch = {
        { "listen",        &Server::setPort        },
        { "host",          &Server::setHost        },
        { "root",          &Server::setRoot        },
        { "index",         &Server::setIndex       },
        { "max_body_size", &Server::setMaxBodySize },
        { "error_page",    &Server::setErrorPages  },
    };
	
	while (input >> word) {
		if (word == "}") {
			break ;
		}
		if (word == "location") {
			result.addLocation(parseLocation(input));
			continue ;
		}
		std::vector<std::string> values = readValues(input);
		std::map<std::string, Setter>::const_iterator iter = dispatch.find(word);
		if (iter != dispatch.end()) {
    		(result.*(iter->second))(values);
		}
		else {
			throw std::runtime_error("Unknown server directive: " + word);
		}
	}
	return result;
}

std::vector<Server> ConfigParser::parseConfig(std::istringstream& input) {
	std::vector<Server> result;
	std::string word;
	
	while (input >> word) {
		if (word == "server") {
			std::string next;
			if (input >> next && next == "{") {
				result.push_back(parseServer(input));
			}
		}
		else if (word == "server{"){
			result.push_back(parseServer(input));
		}
	}
	return result;
}
