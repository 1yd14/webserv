/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_validator.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 11:06:49 by rmhazres          #+#    #+#             */
/*   Updated: 2026/05/27 14:50:12 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include "../src/http/HttpValidator.hpp"
#include "../src/http/HttpParser.hpp"
#include "./test_utils.hpp"


// <-------------------- validating method ------------------->

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


// <-------------------- validating target ------------------->

void test_valid_target()
{
	std::cout << "=================testing valid Target !====================\n";
	
	HttpValidator validator;
	
	HttpParser parser;

	std::string raw = "GET /index.html HTTP/1.1\r\nHost: localhost\r\n\r\n";

	HttpRequest req = parser.parseHttp(raw);
	HttpStatus status = validator.validate(req);
	
	assert_equal_int((int)HttpStatus::OK, (int)status, "tesing valid Target");
}
void test_unsafe_char_target()
{
	std::cout << "=================testing invalid char in Target !====================\n";
	HttpValidator validator;
	
	HttpParser parser;

	std::string raw = "GET /index>..html HTTP/1.1\r\nHost: localhost\r\n\r\n";

	HttpRequest req = parser.parseHttp(raw);
	HttpStatus status = validator.validate(req);
	
	assert_equal_int((int)HttpStatus::BAD_REQUEST, (int)status, "tesing unsafe char");
}

void test_no_slash()
{
	std::cout << "=================testing no slash Target !====================\n";
	HttpValidator validator;
	
	HttpParser parser;

	std::string raw = "GET index.html HTTP/1.1\r\nHost: localhost\r\n\r\n";

	HttpRequest req = parser.parseHttp(raw);
	HttpStatus status = validator.validate(req);
	
	assert_equal_int((int)HttpStatus::BAD_REQUEST, (int)status, "tesing no slash");
}

// <-------------------- validating protocol ------------------->

void test_valid_protocol()
{
	std::cout << "=================testing valid protocol! =====================\n";
	HttpValidator validator;
	
	HttpParser parser;

	std::string raw = "GET /index.html HTTP/1.1\r\nHost: localhost\r\n\r\n";

	HttpRequest req = parser.parseHttp(raw);
	HttpStatus status = validator.validate(req);
	assert_equal_int((int)HttpStatus::OK, (int)status, "tesing valid protocol");

}

// <-------------------- validating Header ------------------->

void test_invalid_protocol()
{
	std::cout << "=================testing invalid protocol! =====================\n";
	HttpValidator validator;
	
	HttpParser parser;

	std::string raw = "GET /index.html HTTP/1.5\r\nHost: localhost\r\n\r\n";

	HttpRequest req = parser.parseHttp(raw);
	HttpStatus status = validator.validate(req);
	assert_equal_int((int)HttpStatus::HTTP_VERSION_NOT_SUPPOERTED, (int)status, "tesing invalid protocol");
}

void test_valid_header()
{
	std::cout << "=================testing valid header! =====================\n";
	HttpValidator validator;
	
	HttpParser parser;

	std::string raw = "POST /index.html HTTP/1.1\r\nHost: \r\nContent-Length: 15\r\n\r\n";

	HttpRequest req = parser.parseHttp(raw);
	HttpStatus status = validator.validate(req);
	assert_equal_int((int)HttpStatus::OK, (int)status,"valid header");
}

void test_content_length()
{
	std::cout << "=================testing content length! =====================\n";
	HttpValidator validator;
	
	HttpParser parser;

	std::string raw = "POST /index.html HTTP/1.1\r\nHost: \r\nContent-Length: 9223372036854775808\r\n\r\n";

	HttpRequest req = parser.parseHttp(raw);
	HttpStatus status = validator.validate(req);
	// assert_equal_int((int)HttpStatus::OK, (int)status,"valid header");
	std::cout << req.getContentLength() << " status is " << (int)status << std::endl;
}



int main()
{
	std::cout << "**********************testing Validator!**********************\n\n";
	test_valid_get();
	test_method();
	test_method_no_ascii();
	test_valid_but_not_allowed();
	test_valid_target();
	test_unsafe_char_target();
	test_no_slash();
	test_valid_target();
	test_invalid_protocol();
	test_valid_header();
	test_content_length();

	return 0;
}