/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parser.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 10:42:19 by rmhazres          #+#    #+#             */
/*   Updated: 2026/05/21 13:22:24 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cassert>
#include <iostream>
#include <string>
#include "../src/common/HttpRequest.hpp"
#include "../src/http/HttpParser.hpp"


void assert_equal_str(const std::string &expected, const std::string &actual, const std::string &label)
{
	if(expected == actual)
	{
		std::cout << "[PASSED] " << label << "\n";		
	}
	else 
	{
		std::cout << "[FAILED] " << label << " - expected: " << expected << " 'got: " << actual <<"'\n";
	}
}
void assert_equal_int(int expected, int actual, const std::string &label)
{
	if(expected == actual)
	{
		std::cout << "[PASSED] " << label << "\n";		
	}
	else 
	{
		std::cout << "[FAILED] " << label << " - expected: " << expected << " 'got: " << actual <<"'\n";
	}
}

void test_valid_get_request()
{
	// Arrange
	HttpParser parser;
	std::string raw = "GET /index.html HTTP/1.1\r\nHost: localhost\r\n\r\n";
	// Act 
	HttpRequest req = parser.parseHttp(raw);
	// Assert
	assert_equal_str("GET", req.getMethod(), "GET");
	assert_equal_str("/index.html", req.getTarget(), "target");
	assert_equal_str("HTTP/1.1", req.getProtocol(), "protocol");
}

void test_empty_request()
{
	HttpParser parser;
	std::string raw;
	HttpRequest req = parser.parseHttp(raw);
	assert_equal_int((int)HttpStatus::BAD_REQUEST, (int)req.getStatusCode(), "empty");
}

void test_missing_first_line()
{
	HttpParser parser;
	std::string raw = "GET /index.html HTTP/1.1\r\nHost: localhost\r\n\r";
	
	HttpRequest req = parser.parseHttp(raw);
	assert_equal_int((int)HttpStatus::BAD_REQUEST, (int)req.getStatusCode(), "missing first line");
	
}

//Todo - test the parser with blocking code

int main()
{
	test_valid_get_request();
	test_empty_request();
	test_missing_first_line();
	return 0;	
}