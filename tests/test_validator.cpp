/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_validator.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 11:06:49 by rmhazres          #+#    #+#             */
/*   Updated: 2026/05/22 16:20:25 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include "../src/http/HttpValidator.hpp"
#include "../src/http/HttpParser.hpp"
#include "./test_utils.hpp"

void test_valid_get()
{
	std::cout << "=================testing Valid method!====================\n";

	HttpValidator validator;

	HttpParser parser;
	std::string raw = "GET /index.html HTTP/1.1\r\nHost: localhost\r\n\r\n";
	HttpRequest req = parser.parseHttp(raw);

	HttpStatus status = validator.validate(req);
	
	assert_equal_int((int)HttpStatus::OK, (int)status, "valid method");
}

void test_method()
{
	std::cout << "=================testing invalid method!====================\n";

	HttpValidator validator;

	HttpParser parser;
	std::string raw = "get /index.html HTTP/1.1\r\nHost: localhost\r\n\r\n";
	HttpRequest req = parser.parseHttp(raw);

	HttpStatus status = validator.validate(req);
	
	assert_equal_int((int)HttpStatus::BAD_REQUEST, (int)status, "invalid lowercase method");
}
void test_method_no_ascii()
{
	std::cout << "=================testing invalid ascii in method !====================\n";

	HttpValidator validator;

	HttpParser parser;
	std::string raw = "G@T /index.html HTTP/1.1\r\nHost: localhost\r\n\r\n";
	HttpRequest req = parser.parseHttp(raw);

	HttpStatus status = validator.validate(req);
	
	assert_equal_int((int)HttpStatus::BAD_REQUEST, (int)status, "invalid char");
}
void test_valid_but_not_allowed()
{
	std::cout << "=================testing valid but not allowed !====================\n";

	HttpValidator validator;

	HttpParser parser;
	std::string raw = "PUT /index.html HTTP/1.1\r\nHost: localhost\r\n\r\n";
	HttpRequest req = parser.parseHttp(raw);

	HttpStatus status = validator.validate(req);
	
	assert_equal_int((int)HttpStatus::METHOD_NOT_ALLOWED, (int)status, "not allowed");
}




int main()
{
	std::cout << "**********************testing Validator!**********************\n\n";
	test_valid_get();
	test_method();
	test_method_no_ascii();
	test_valid_but_not_allowed();
	return 0;
}