/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 16:44:38 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/08/10 17:28:32 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"
#include "EventLoop.hpp"
#include <algorithm>
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
#include "../http/HttpResponseBuilder.hpp"
#include "../http/HttpValidator.hpp"


uint64_t Connection::s_nextId = 1;

Connection::Connection(int fd, const Server& server, const int& port) : ASocket(fd), _server(server), _localPort(port), _state(READING), _lastActivity(time(nullptr)), _id(s_nextId++) {

}

Connection::~Connection() {}

void Connection::setState(State newState) {
	_state = newState;
}

State Connection::getState() const {
	return _state;
}

int Connection::getLocalPort() const {
	return _localPort;
}

time_t Connection::getLastActivity() const {
	return _lastActivity;
}
//this function needs to check if everything is read or if more needs to be read to change the epoll event that triggers waking up from EPOLLIN TO EPOLLOUT

std::string extractTarget(const std::string& buffer)
{
	size_t lineEnd = buffer.find("\r\n");
	if (lineEnd == std::string::npos)
	{
		return "";
	}

	std::string requestLine = buffer.substr(0, lineEnd);
	size_t first = requestLine.find(" ");
	size_t second = requestLine.find(" ", first + 1);

	if (first == std::string::npos || second == std::string::npos)
	{
		return "";
	}

	return requestLine.substr(first + 1, second - (first + 1));
}

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
	if (_readBuffer.size() > 16384 && _readBuffer.find("\r\n\r\n") == std::string::npos)
    {

        _pendingError = "HTTP/1.1 414 URI Too Long\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
        _state = ERROR_PENDING;
        _readBuffer.clear();
		loop.setWriting(this, EPOLL_CTL_MOD);
        return;
    }
	
	if (_state == ERROR_PENDING)
	{
		handleErrorPending(loop);
		return;
	}
	std::string target = extractTarget(_readBuffer);
	const LocationBlock* block = findMatchingLocation(target, _server);
	if (!isRequestComplete(block))
	{
		return;
	}
	if (_state == ERROR_PENDING)
	{
		_writeBuffer = _pendingError;
		_readBuffer.clear();
		_state = WRITING;
		loop.setWriting(this, EPOLL_CTL_MOD);
		return;
	}
	std::string requestToParse = prepareRequest();
		//  std::cout << "==============REQUEST================" << std::endl;
		// std::cout << requestToParse << std::endl;
		// std::cout << "==============================" << std::endl;
	if (_state == ERROR_PENDING)
	{
		_writeBuffer = _pendingError;
		_readBuffer.clear();
		_state = WRITING;
		loop.setWriting(this, EPOLL_CTL_MOD);
		return;
	}
	if (requestToParse.empty())
	{
		return;
	}
	dispatch(requestToParse, loop);
}

bool Connection::isRequestComplete(const LocationBlock * block)
{
	size_t headerEnd = _readBuffer.find("\r\n\r\n");

	if (headerEnd == std::string::npos)
	{
		return false;
	}
	
	if (_readBuffer.find("Transfer-Encoding: chunked") != std::string::npos)
	{
		return  _readBuffer.find("0\r\n\r\n") != std::string::npos || _readBuffer.find("0\r\n") != std::string::npos;
	}
	
	size_t maxBody = (block != nullptr && block->getMaxBodySize().has_value())
	    ? block->getMaxBodySize().value()
	    : (size_t)_server.getMaxBodySize();

	size_t contentLength = extractContentLength(_readBuffer);
	
	if (contentLength > maxBody)
	{
		_pendingError = "HTTP/1.1 413 Payload Too Large\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
    	_state = ERROR_PENDING;
    	_readBuffer.clear();
    	return true;
		
	}

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
	bool error = false;
	size_t headerEnd = _readBuffer.find("\r\n\r\n");
	size_t totalExpected = 0;
	if (_readBuffer.find("Transfer-Encoding: chunked") != std::string::npos)
	{
		size_t chunkEnd = _readBuffer.find("0\r\n\r\n");
        if (chunkEnd == std::string::npos)
		{
            return "";
		}
        totalExpected = chunkEnd + 5;
		totalExpected = _readBuffer.find("0\r\n\r\n") + 5;
	}
	else
	{
		size_t contentLength = extractContentLength(_readBuffer);
	
		totalExpected = headerEnd + 4 + contentLength;
		if (_readBuffer.size() < totalExpected)
		{	
			return "";
		}
	}
	std::string requestToParse = _readBuffer.substr(0, totalExpected);
	_readBuffer.erase(0, totalExpected);
	
	if(requestToParse.find("Transfer-Encoding: chunked") != std::string::npos)
	{
		size_t bodyStart = requestToParse.find("\r\n\r\n") + 4;
		std::string unchuncked = unchunkBody(requestToParse.substr(bodyStart), error);
		if(error)
		{
			
        _pendingError = "HTTP/1.1 400 Bad request\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
        _state = ERROR_PENDING;
        _readBuffer.clear();
        return " ";
		}
		requestToParse = requestToParse.substr(0, bodyStart) + unchuncked;
	}
	return  requestToParse;
}

void Connection::dispatch(const std::string& requestToParse, EventLoop& loop)
{

	HttpResponseBuilder builder;
	HttpRequest request = HttpParser::parseHttp(requestToParse);
	HttpStatus status = request.getStatusCode();

	if(status == HttpStatus::OK ||status == HttpStatus::NONE)
	{
		HttpValidator validator;
		
		status = validator.validate(request,_server);
		if (status == HttpStatus::OK)
		{
			Router router;

			if(router.route(request,_server) == RouteType::CGI)
			{
				_state = AWAITING_CGI;
				CGIHanlder::execute(request, _server, loop ,*this);
				return;
			}
			_writeBuffer = processRequest(request, _server);
			std::string lowerBuffer = _writeBuffer;
			std::transform(
					lowerBuffer.begin(),
					lowerBuffer.end(),
					lowerBuffer.begin(),
					[](unsigned char c) {
					    return std::tolower(c);
					}
				);
			if (lowerBuffer.find("connection: close") != std::string::npos)
				{
					_shouldClose = true;
				}
			_state = WRITING;
			loop.setWriting(this, EPOLL_CTL_MOD);
			return;
		}
	}
	HttpResponse response = builder.build(request, _server, RouteType::NOT_FOUND);
	response.setHeader("Connection", "close");
	_writeBuffer = response.serialize();
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
	if (_writeBuffer.empty())
	{
		if (_shouldClose)
		{
			loop.removeConnection(getFd());
		}
		else {
			_state = READING;
			loop.setReading(this, EPOLL_CTL_MOD);
		}
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

void Connection::onTimeout(EventLoop & loop) {
	if (_state == READING && !_readBuffer.empty())
	{
		std::string timeoutResponse = "HTTP/1.1 408 Request Timeout\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
		send(getFd(), timeoutResponse.c_str(), timeoutResponse.size(), 0);
	}
	loop.removeConnection(getFd());
}

void Connection::setWriterBuffer(const std::string& data)
{
	_writeBuffer = data;
}

uint64_t Connection::getId() {
	return (_id);
}
