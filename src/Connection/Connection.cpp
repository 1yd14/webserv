/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 16:44:38 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/06/10 13:25:23 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"
#include "EventLoop.hpp"
#include <ctime>
#include <sys/socket.h>
#include <iostream>
#include <sys/types.h>


Connection::Connection(int fd, const Server& server) : ASocket(fd), _server(server), _state(READING), _lastActivity(time(nullptr)) {
	_writeBuffer = "HTTP/1.1 200 OK\r\n"
			"Content-Length: 13\r\n"
			"\r\n"
			"Hello, World!";
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

void Connection::handleRead(EventLoop &loop) {
	char buffer[4096];
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
		_writeBuffer = "HTTP/1.1 200 OK\r\n"
			"Content-Length: 13\r\n"
			"\r\n"
			"Hello, World!";
		_state = WRITING;
	}
}

void Connection::handleWrite(EventLoop &loop) {
	ssize_t bytes = send(getFd(), _writeBuffer.c_str(), _writeBuffer.size(), 0);
	if (bytes == -1) {
		loop.removeConnection(getFd());
		return ;
	}
	_lastActivity = time(nullptr);
	_writeBuffer.erase(0, bytes);
	if (_writeBuffer.empty()) {
		_state = READING;
	}
}

void Connection::handleEvent(EventLoop &loop) {
	
	_lastActivity = time(nullptr);
	if (_state == READING) {
		handleRead(loop);
	}
	if (_state == WRITING) {
		handleWrite(loop);
	}
	(void)_server;
}

