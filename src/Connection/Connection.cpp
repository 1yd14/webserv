/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 16:44:38 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/07/07 15:04:38 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"
#include "EventLoop.hpp"
#include <cstddef>
#include <ctime>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <iostream>
#include <sys/types.h>
#include "../http/HttpPipeline.hpp"
#include "../http/HttpParser.hpp"
#include "../common/Utils.hpp"
#include "../http/Router.hpp"
#include "../http/CGiHandler.hpp"


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
//this function needs to check if everything is read or if more needs to be read to change the epoll event that triggers waking up from EPOLLIN TO EPOLLOUT

void Connection::handleRead(EventLoop &loop) {
	char buffer[4096];
	std::cout << "handleRead called\n";

	ssize_t bytes = recv(getFd(), buffer, sizeof(buffer), 0);
	std::cout << "recv returned: " << bytes << "\n";
	if (bytes == 0) {
		loop.removeConnection(getFd());
		return;
	}
	if (bytes == -1) {
		loop.removeConnection(getFd());
		return;
	}
	_readBuffer.append(buffer, bytes);
	size_t headerEnd = _readBuffer.find("\r\n\r\n");
	if (headerEnd == std::string::npos) 
	{
		return;
	}		
	size_t totalExpected = headerEnd + 4 + extractContentLength(_readBuffer);
	
	if(_readBuffer.size() < totalExpected)
	{
		return;
	}

		HttpRequest request = HttpParser::parseHttp(_readBuffer);
		Router router;
		if (router.route(request,_server) == RouteType::CGI)
		{
			CGIHanlder::execute(request,_server, loop, *this);
			return;
		}
		_writeBuffer = processRequest(_readBuffer, _server);
		std::cout << "switching to WRITING, buffer size=" << _writeBuffer.size() << "\n";
		_state = WRITING;
		loop.setWriting(this, EPOLL_CTL_MOD);
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
		loop.setReading(this, EPOLL_CTL_MOD);
	}
}

void Connection::handleEvent(EventLoop &loop) {
	
	_lastActivity = time(nullptr);
	if (_state == READING) {
		handleRead(loop);
	}
	else if (_state == WRITING) {
		handleWrite(loop);
	}
	(void)_server;
}


void Connection::setWriterBuffer(const std::string& data)
{
	_writeBuffer = data;
}
