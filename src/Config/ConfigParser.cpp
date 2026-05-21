/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 13:32:34 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/05/21 15:40:14 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"
#include "Server.hpp"
#include <sstream>
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
		//if (word == "location") {
		//	std::string next;
        //    if (input >> next && next == "{") {
        //        //result.addPath(parseLocation(input));
		//	}
        //    continue;
		//}
		std::vector<std::string> values = readValues(input);
		std::map<std::string, Setter>::const_iterator iter = dispatch.find(word);
		if (iter != dispatch.end()) {
    		(result.*(iter->second))(values);
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
