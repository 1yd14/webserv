/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 19:45:49 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/06/04 17:24:50 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ListeningSocket.hpp"
#include <stdexcept>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

ListeningSocket::ListeningSocket(const Server& server) :
		ASocket(socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)), _server(server) {
	if (getFd() == -1) {
		throw std::runtime_error("socket fd is not created");
	}
	int opt = 1;
	setsockopt(getFd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	bindSocket();
	listenSocket();
}


void ListeningSocket::bindSocket() {
	sockaddr_in addr{};

	addr.sin_family = AF_INET;
	addr.sin_port = htons(_server.getPort());
	addr.sin_addr.s_addr = inet_addr(_server.getHost().c_str()); 
	if (bind(getFd(),
			reinterpret_cast<sockaddr*>(&addr),
			sizeof(addr)) == -1)
	{
		throw std::runtime_error("bind failed");
	}
}

void ListeningSocket::listenSocket()
{
	if (listen(getFd(), SOMAXCONN) == -1) {
		throw std::runtime_error("listen failed");
	}
}

void ListeningSocket::handleEvent() const {
    // temporary stub
}
