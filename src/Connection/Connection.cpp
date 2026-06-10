/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 16:44:38 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/06/10 12:55:00 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"
#include "EventLoop.hpp"
#include <ctime>
#include <sys/socket.h>
#include <iostream>


Connection::Connection(int fd, const Server& server) : ASocket(fd), _server(server), _state(READING), _lastActivity(time(nullptr)) {
}

Connection::~Connection() {}

void Connection::setState(State newState) {
	_state = newState;
}

State Connection::getState() {
	return _state;
}

time_t Connection::getLastActivity() const {
	return _lastActivity;
}

void Connection::handleEvent(EventLoop &loop) {
    char buffer[4096];
	
	_lastActivity = time(nullptr);
	ssize_t bytes = recv(getFd(), buffer, sizeof(buffer), 0);
	if (bytes == 0) {
		loop.removeConnection(getFd());
		return;
	}
	if (bytes == -1) {
		loop.removeConnection(getFd());
		return;
	}
	_readBuffer.append(buffer, bytes);
	if (_readBuffer.find("\r\n\r\n") != std::string::npos) {
		// full request received, ready to process
		std::cout << _readBuffer << std::endl;
		_state = WRITING;
	}
	(void)_server;
}

