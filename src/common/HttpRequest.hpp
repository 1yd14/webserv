/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 15:20:41 by rmhazres          #+#    #+#             */
/*   Updated: 2026/05/19 12:10:33 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <map>
#include <string>


class HttpRequest 
{
	public:
		HttpRequest();
		~HttpRequest();
		HttpRequest(const HttpRequest &other);
		HttpRequest& operator=(const HttpRequest &other);

		std::string getMethod() const;
		std::string getTarget() const;
		std::string getProtocol() const;
		std::map<std::string, std::string> getHeader() const;
		std::string getBody() const;

		void setMethod(const std::string& value);
		void setTarget(const std::string& value);
		void setProtocol(const std::string& value);
		void setHeader(const std::map<std::string, std::string>& value);
		void setBody(const std::string& value);

	private:
		std::string _method;
		std::string _target;
		std::string _protocol;
		std::map<std::string, std::string> _header;
		std::string _body;
};