/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 13:23:49 by rmhazres          #+#    #+#             */
/*   Updated: 2026/07/08 16:10:54 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>

#include "../common/HttpRequest.hpp"


/* still need to decide on the implementation of this class !
	does it copy or not ?
*/ 
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

