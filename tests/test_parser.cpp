/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parser.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 10:42:19 by rmhazres          #+#    #+#             */
/*   Updated: 2026/05/22 11:23:50 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cassert>
#include <iostream>
#include <string>
#include "../src/common/HttpRequest.hpp"
#include "../src/http/HttpParser.hpp"
#include "./test_utils.hpp"

void test_valid_get_request()
{
	std::cout << "=================testing valid get request====================\n";
	HttpParser parser;
	std::string raw = "GET /index.html HTTP/1.1\r\nHost: localhost\r\n\r\n";
	HttpRequest req = parser.parseHttp(raw);
	assert_equal_str("GET", req.getMethod(), "GET");
	assert_equal_str("/index.html", req.getTarget(), "target");
	assert_equal_str("HTTP/1.1", req.getProtocol(), "protocol");
	assert_equal_int( 0,(int)req.getStatusCode(), "Code");

}
void test_valid_post_request()
{
	std::cout << "=================testing valid post request====================\n";

	HttpParser parser;
	std::string raw = "POST /index.html HTTP/1.1\r\nHost: localhost\r\n\r\n";
	HttpRequest req = parser.parseHttp(raw);
	assert_equal_str("POST", req.getMethod(), "POST");
	assert_equal_str("/index.html", req.getTarget(), "target");
	assert_equal_str("HTTP/1.1", req.getProtocol(), "protocol");
}
void test_empty_request()
{
	std::cout << "=================testing empty request====================\n";

	HttpParser parser;
	std::string raw;
	HttpRequest req = parser.parseHttp(raw);
	assert_equal_int((int)HttpStatus::BAD_REQUEST, (int)req.getStatusCode(), "empty");
}

void test_missing_first_line()
{
	std::cout << "=================testing missing first line====================\n";

	HttpParser parser;
	std::string raw = "GET /index.html HTTP/1.1\r\nHost: localhost";
	
	HttpRequest req = parser.parseHttp(raw);
	assert_equal_int((int)HttpStatus::BAD_REQUEST, (int)req.getStatusCode(), "missing first line");
}


void test_post_with_content()
{
	std::cout << "=================testing post with content====================\n";
	
	HttpParser parser;
	std::string raw ="GET /index.html HTTP/1.1\r\nHost: localhost\r\n\r\n field1=value1&field2=value2";

	HttpRequest req = parser.parseHttp(raw);
	assert_equal_str("field1=value1&field2=value2", req.getBody(), "post with content");
}
//this doesnt make sens
// void test_with_missing_parts()
// {
// 	std::cout << "=================testing missing some parts ====================\n";	
// 	HttpParser parser;
// 	std::string raw ="GET/index.html HTTP/1.1\r\nHost: localhost\r\n\r\n field1=value1&field2=value2";

// 	HttpRequest req = parser.parseHttp(raw);
// 	assert_equal_int((int)HttpStatus::BAD_REQUEST, (int)req.getStatusCode(), "missing parts");


// }
int main()
{
	test_valid_get_request();
	test_empty_request();
	test_missing_first_line();
	test_valid_post_request();
	// test_multiple_headers();
	test_post_with_content();
	// test_with_missing_parts();


	return 0;
}