/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 13:23:49 by rmhazres          #+#    #+#             */
/*   Updated: 2026/08/12 12:36:25 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>

#include "../Common/HttpRequest.hpp"

class HttpParser
{
	public:
				HttpParser();
				~HttpParser();
				HttpParser(const HttpParser &other) = delete;
				HttpParser& operator=(const HttpParser &other) = delete;
[[nodiscard]]	static HttpRequest parseHttp(const std::string &rawRequest);

	private:
				static void extractFirstLine(const std::string &line, HttpRequest &req);
				static void extractHeaders(const std::string &line, HttpRequest &req);
				static void extractContentLength(HttpRequest &req);
				static long convertContentLength(const std::string &str);
		
};

