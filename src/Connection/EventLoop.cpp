/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 16:53:28 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/07/14 16:45:30 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EventLoop.hpp"
#include <algorithm>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <sys/epoll.h>
#include "../Signals/signals.hpp"
#include "Connection.hpp"
#include <array>
#include <vector>

#define MAX_EVENTS 1024
#define TIMEOUT_MS 5000
#define TIMEOUT_SECONDS 60

EventLoop::EventLoop() {
	_epollFd = epoll_create1(0);
	if (_epollFd == -1) {
		throw std::runtime_error("epoll cannot be created");
	}
}
EventLoop::~EventLoop() {
	close(_epollFd);
}

// this function throws an exception because these ones are added before creating up epoll loop
void EventLoop::addListeningSocket(std::unique_ptr<ASocket> socket) {
	if (!setReading(socket.get(), EPOLL_CTL_ADD)) {
		throw std::runtime_error("epoll_ctl failed: " + std::string(strerror(errno)));
	}
	_listeners.push_back(std::move(socket));
}

// this function is similar to addListeningSocket but does not throw an exception because of time sensitivity
void EventLoop::addConnection(std::unique_ptr<Connection> connection) {
	if (!setReading(connection.get(), EPOLL_CTL_ADD)) {
		std::cerr << "epoll_ctl failed for connection: " << strerror(errno) << "\n";
		return;
	}
	_connections.push_back(std::move(connection));
}

void EventLoop::addCgi(std::unique_ptr<CGIProcess> cgiProcess)
{
	if (!setReading(cgiProcess.get(), EPOLL_CTL_ADD)) {
		std::cerr << "epoll_ctl failed for connection: " << strerror(errno) << "\n";
		return;
	}
	_cgiProcesses.push_back(std::move(cgiProcess));
}

void EventLoop::run() {
	std::array<epoll_event, MAX_EVENTS> events;

	
	while(g_sig_val == 0) {
		int readyFds = epoll_wait(_epollFd, events.data(), events.size(), TIMEOUT_MS);
		if (readyFds < 0) {
			if (errno == EINTR) {
				break ;
			}
			//hanlde the error
			continue;
		}
		for (int i = 0; i < readyFds; ++i) {
			auto* socket = static_cast<ASocket*>(events[i].data.ptr);
			if ((events[i].events & (EPOLLHUP | EPOLLERR | EPOLLRDHUP)) != 0) {
				if (isCGIProcess(socket->getFd()))
				{
					socket->handleEvent(*this);
					continue;
				}
				else {
					removeConnection(socket->getFd());
					continue;
				}
			}
			socket->handleEvent(*this);
		}
		std::vector<int> timeoutFds;
		for (size_t i = 0; i < _connections.size(); i++) {
			if(time(nullptr) - _connections[i]->getLastActivity() > TIMEOUT_SECONDS) {
				timeoutFds.push_back(_connections[i]->getFd());
			}
		}
		for (int fd : timeoutFds) {
			removeConnection(fd);
		}
	}
}

void EventLoop::removeConnection(int fd) {
	epoll_ctl(_epollFd, EPOLL_CTL_DEL, fd, nullptr);
	_connections.erase(
		std::remove_if(_connections.begin(), _connections.end(), 
			[fd](const std::unique_ptr<Connection> &s){
				return s->getFd() == fd;
			}),
		_connections.end());
}
void EventLoop::removeCGIProcess(int fd)
{
	std::cout << "removing cgi for fd: " << fd << std::endl;
		epoll_ctl(_epollFd, EPOLL_CTL_DEL, fd, nullptr);
	_cgiProcesses.erase(
		std::remove_if(_cgiProcesses.begin(), _cgiProcesses.end(), 
			[fd](const std::unique_ptr<CGIProcess> &s){
				return s->getFd() == fd;
			}),
		_cgiProcesses.end());
}

Connection *EventLoop::getConnection(int fd) {
	for (auto &c : _connections) {
		if (c->getFd() == fd) {
			return c.get();
		}
	}
	return nullptr;
}

bool EventLoop::setReading(ASocket *socket, int op) const {
	epoll_event event{};
	event.events = EPOLLIN | EPOLLHUP | EPOLLERR | EPOLLRDHUP;
	event.data.ptr = socket;
	return epoll_ctl(_epollFd, op, socket->getFd(), &event) != -1;
}

bool EventLoop::setWriting(ASocket *socket, int op) const {
	epoll_event event{};
	event.events = EPOLLOUT | EPOLLHUP | EPOLLERR | EPOLLRDHUP;
	event.data.ptr = socket;
	bool result = (epoll_ctl(_epollFd, op, socket->getFd(), &event) != -1);
	return result;
}

bool EventLoop::isCGIProcess(int fd)
{
	for( auto&& process : _cgiProcesses)
	{
		if (process->getFd() == fd)
		{
			return true;
		}
	}
	return false;
}