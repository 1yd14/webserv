/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 16:44:38 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/07/10 18:09:01 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"
#include "EventLoop.hpp"
#include <cstddef>
#include <ctime>
#include <string>
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

void Connection::handleRead(EventLoop& loop)
{
	char buffer[4096];
	
	ssize_t bytes = recv(getFd(), buffer, sizeof(buffer), 0);
	if (bytes <= 0)
	{
		loop.removeConnection(getFd());
		return;
	}
	
	_readBuffer.append(buffer,bytes);
	  if (_readBuffer.size() > 8192 && _readBuffer.find("\r\n\r\n") == std::string::npos)
    {
		 	std::cout << "=basla=" << std::endl;

        _pendingError = "HTTP/1.1 414 URI Too Long\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
        _state = ERROR_PENDING;
        _readBuffer.clear();
        return;
    }
	
	if (_state == ERROR_PENDING)
	{
		handleErrorPending(loop);
		return;		
	}
	if (!isRequestComplete())
	{
		return;
	}
	std::string requestToParse = prepareRequest();
 	std::cout << "==============REQUEST================" << std::endl;
	std::cout << requestToParse << std::endl;
	std::cout << "==============================" << std::endl;
	
	dispatch(requestToParse, loop);
	std::cout << "==============RESPONSE================" << std::endl;
	std::cout << _writeBuffer << std::endl;
	std::cout << "==============================" << std::endl;
 	
}

bool Connection::isRequestComplete()
{
	size_t headerEnd = _readBuffer.find("\r\n\r\n");
	if (headerEnd == std::string::npos)
	{
		return false;
	}
	if (_readBuffer.find("Transfer-Encoding: chunked") != std::string::npos)
	{
		return  _readBuffer.find("0\r\n\r\n") != std::string::npos;
	}
	size_t contentLength = extractContentLength(_readBuffer);
	return  _readBuffer.size() >= headerEnd + 4 + contentLength;
}

void Connection::handleErrorPending(EventLoop& loop)
{
	_lastActivity = time(nullptr); 
	if (_readBuffer.find("\r\n\r\n") != std::string::npos)
	{
		_writeBuffer = _pendingError;
		_readBuffer.clear();
		_state = WRITING;
		loop.setWriting(this, EPOLL_CTL_MOD);
	}
	else {
		_readBuffer.clear();
	}
}

std::string Connection::prepareRequest()
{
	size_t headerEnd = _readBuffer.find("\r\n\r\n");
	size_t totalExpected;
	if (_readBuffer.find("Transfer-Encoding: chuncked") != std::string::npos)
	{
		totalExpected = _readBuffer.find("0\r\n\r") + 5;
	}
	else
	{
		totalExpected = headerEnd + 4 + extractContentLength(_readBuffer);
	}
	std::string requestToParse = _readBuffer.substr(0, totalExpected);
	_readBuffer.erase(0, totalExpected);
	
	if(requestToParse.find("Transfer-Encoding: chuncked") != std::string::npos)
	{
		size_t bodyStart = requestToParse.find("\r\n\r\n") + 4;
		std::string unchuncked = unchunkBody(requestToParse.substr(bodyStart));
		requestToParse = requestToParse.substr(0, bodyStart) + unchuncked;
	}
	return  requestToParse;
}

void Connection::dispatch(const std::string& requestToParse, EventLoop& loop)
{
	// if(requestToParse.find("\r\n") != std::string::npos)
	// {
	// 	size_t firstLine = requestToParse.find("\r\n");
	// 	if (firstLine > 8192)
	// 	{
	// 		_pendingError = "HTTP/1.1 414 URI Too Long\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
	// 		_state = ERROR_PENDING;
	// 		return;
	// 	} 
	// }

	HttpRequest request = HttpParser::parseHttp(requestToParse);
	Router router;
	if (router.route(request, _server) == RouteType::CGI)
	{
		CGIHanlder::execute(request,_server, loop, *this);
		return;
	}
	_writeBuffer = processRequest(requestToParse, _server);
	
	_state = WRITING;
	loop.setWriting(this, EPOLL_CTL_MOD);
}







	// void Connection::handleRead(EventLoop &loop) {
	
// 	char buffer[4096];

	
// 	ssize_t bytes = recv(getFd(), buffer, sizeof(buffer), 0);
// 	if (bytes == 0) {
// 		loop.removeConnection(getFd());
// 		return;
// 	}
// 	if (bytes == -1) {
// 		loop.removeConnection(getFd());
// 		return;
// 	}
// 	_readBuffer.append(buffer, bytes);
	
// 	if (_state == ERROR_PENDING)
// 	{
// 		_readBuffer.append(buffer, bytes);
// 		if (_readBuffer.find("\r\n\r\n") != std::string::npos)
// 		{
// 			_writeBuffer = _pendingError;
// 			_readBuffer.clear();
// 			_state = WRITING;
// 			loop.setWriting(this, EPOLL_CTL_MOD);
// 		}
// 		else
// 			{_readBuffer.clear(); // keep discarding
// 			}
// 		return;
// 	}	
// 	if (_readBuffer.size() > 1042) // URI limit
// 	{
// 		_writeBuffer = "HTTP/1.1 414 URI Too Long\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
// 		_readBuffer.clear();
// 		_state = WRITING;
// 		loop.setWriting(this, EPOLL_CTL_MOD);
// 		return;
// 	}
// 	std::cout << "Buffer after append: '" << _readBuffer << "'\n";

// 	if (_readBuffer.find("HTTP/") == std::string::npos)
// 	{
// 		_readBuffer.clear();
// 		return;
// 	}
// 	size_t headerEnd = _readBuffer.find("\r\n\r\n");
// 	if (headerEnd == std::string::npos) 
// 	{
// 		return;
// 	}
// 	size_t totalExpected;
// 	std::string lowerBuffer = _readBuffer;
// 	// std::transform(lowerBuffer.begin(), lowerBuffer.end(), lowerBuffer.begin(), ::tolower);

// 	if (_readBuffer.find("Transfer-Encoding: chunked") != std::string::npos)
// 	{
// 		if (_readBuffer.find("0\r\n\r\n") == std::string::npos)
// 		{
// 			std::cout << "Chunked check — waiting for terminator\n";
// 			return;
// 		}
// 		totalExpected = _readBuffer.find("0\r\n\r\n") + 5;
// 	}
// 	else
// 	{
// 		totalExpected = headerEnd + 4 + extractContentLength(_readBuffer);
// 	}

// 	if(_readBuffer.size() < totalExpected)
// 	{
// 		return;
// 	}

// 	std::string requestToParse = _readBuffer.substr(0, totalExpected);

// 	if (requestToParse.find("transfer-encoding: chunked") != std::string::npos)
// 	{
// 		// find body start
// 		size_t bodyStart = requestToParse.find("\r\n\r\n") + 4;
// 		std::string chunkedBody = requestToParse.substr(bodyStart);
// 		std::string unchunked = unchunkBody(chunkedBody);
// 		requestToParse = requestToParse.substr(0, bodyStart) + unchunked;

// 	}

// 	HttpRequest request = HttpParser::parseHttp(requestToParse);
// 	Router router;
// 	if (router.route(request,_server) == RouteType::CGI)
// 	{
// 		CGIHanlder::execute(request,_server, loop, *this);
// 		return;
// 	}

// 	_writeBuffer = processRequest(requestToParse, _server);
	
// 	// temporary
// 		if (request.getMethod() == "HEAD")
// 	{
// 		size_t headerEnd = _writeBuffer.find("\r\n\r\n");
// 		if (headerEnd != std::string::npos)
// 		{
// 			_writeBuffer = _writeBuffer.substr(0, headerEnd + 4);
			
// 		}
		
// 		// replace Content-Length with 0
// 		size_t clPos = _writeBuffer.find("Content-Length:");
// 		if (clPos != std::string::npos)
// 		{
// 			size_t clEnd = _writeBuffer.find("\r\n", clPos);
// 			_writeBuffer.replace(clPos, clEnd - clPos, "Content-Length: 0");
// 		}
// 	}
// 	// temporrary ^^^^^
	
// 	_readBuffer.erase(0, totalExpected);
// 	std::cout << "==============REQUEST================" << std::endl;
// 	std::cout << requestToParse << std::endl;
// 	std::cout << "==============================" << std::endl;
// 	std::cout << "==============RESPONSE================" << std::endl;
// 	std::cout << _writeBuffer << std::endl;
// 	std::cout << "==============================" << std::endl;
// 	_state = WRITING;
// 	loop.setWriting(this, EPOLL_CTL_MOD);
// }

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
