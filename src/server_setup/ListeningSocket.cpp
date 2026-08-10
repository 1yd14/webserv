/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 19:45:49 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/08/09 15:53:17 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ListeningSocket.hpp"
#include "../Connection/EventLoop.hpp"
#include "../Connection/Connection.hpp"
#include <memory>
#include <stdexcept>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>

ListeningSocket::ListeningSocket(const Server& server, const int& port) :
		ASocket(socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)), _server(server), _port(port) {
	if (getFd() == -1) {
		throw std::runtime_error("socket fd is not created");
	}
	int opt = 1;
	setsockopt(getFd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	bindSocket(port);
	listenSocket();
}


void ListeningSocket::bindSocket(int port) {
	sockaddr_in addr{};

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(_server.getHost().c_str()); 
	if (addr.sin_addr.s_addr == INADDR_NONE) {
		throw std::runtime_error("invalid IP address: " + _server.getHost());
	}
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

void ListeningSocket::handleEvent(EventLoop &loop) {
	int clientFd = accept(getFd(), nullptr, nullptr);
	fcntl(clientFd, F_SETFD, O_NONBLOCK);
	if (clientFd == -1) {
		std::cerr << "accept failed: " << strerror(errno) << "\n";
		return;
	}
	loop.addConnection(std::make_unique<Connection>(clientFd, _server, _port));
}

void ListeningSocket::onTimeout(EventLoop& loop)
{
    (void)loop; 
}
