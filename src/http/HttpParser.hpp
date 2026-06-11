/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 13:23:49 by rmhazres          #+#    #+#             */
/*   Updated: 2026/06/09 10:03:02 by rmhazres         ###   ########.fr       */
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
[[nodiscard]]	HttpRequest parseHttp(const std::string &rawRequest) const;

	private:
				static void extractFirstLine(const std::string &line, HttpRequest &req);
				static void extractHeaders(const std::string &line, HttpRequest &req);
				static void extractContentLength(HttpRequest &req);
				static long convertContentLength(const std::string &str);
		
};

