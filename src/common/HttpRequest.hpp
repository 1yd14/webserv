/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 15:20:41 by rmhazres          #+#    #+#             */
/*   Updated: 2026/05/19 11:29:45 by rmhazres         ###   ########.fr       */
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

		const std::string getMethod() const;
		const std::string getTarget() const;
		const std::string getProtocol() const;
		const std::map<std::string, std::string> getHeader() const;
		const std::string getBody() const;

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