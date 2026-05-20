/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 13:23:49 by rmhazres          #+#    #+#             */
/*   Updated: 2026/05/20 13:00:19 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>

#include "../common/HttpRequest.hpp"


/* still need to decide on the implementation of this class !
	does it copy or not ?
*/

// TODO: check _statusCode once HttpRequest PR is merged
class HttpParser
{
	public:
		HttpParser();
		~HttpParser();
		HttpParser(const HttpParser &other) = delete;
		HttpParser& operator=(const HttpParser &other) = delete;
		HttpRequest parseHttp(const std::string &rawRequest) const;

		static void extractFirstLine(const std::string &line, HttpRequest &req);
		static void extractHeaders(const std::string &line, HttpRequest &req);
		private:
		
};

