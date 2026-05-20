/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parser.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 10:42:19 by rmhazres          #+#    #+#             */
/*   Updated: 2026/05/20 16:05:51 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <iostream>
#include <string>
#include "../src/common/HttpRequest.hpp"
#include "../src/http/HttpParser.hpp"



//Todo - test the parser with blocking code

int main()
{
	HttpRequest req;
	HttpParser parser;
	const std::string  text = "GET /index.html HTTP/1.1\r\nHost  : localhost\r\nContent-Type: application/json\r\nContent-Length: 32\r\n\r\n";

	req = parser.parseHttp(text);
	std::cout << "====================================== Patser tester ============================\n";
	std::cout << "====================================== testing First line ============================\n";

	std::cout<<" method is :" << req.getMethod() << " target is :" << req.getTarget() << " protcol is :" << req.getProtocol() << (int)req.getStatusCode() << std::endl;

	std::cout << "====================================== testing Header ============================\n";
	std::map<std::string, std::string> header = req.getHeader();
	std::map<std::string, std::string>::iterator itt;
	for (itt = header.begin(); itt != header.end(); itt++)
	{
		std::cout << itt->first << ":" << itt->second << std::endl;
	}
	return 0;
	
}