/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 16:53:28 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/06/10 13:11:15 by lyvan-de         ###   ########.fr       */
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
	epoll_event event{};
	event.events = EPOLLIN;
	event.data.ptr = socket.get();
	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, socket->getFd(), &event) == -1) {
		throw std::runtime_error("epoll_ctl failed: " + std::string(strerror(errno)));
	}
	_listeners.push_back(std::move(socket));
}

// this function is similar to addListeningSocket but does not throw an exception because of time sensitivity
void EventLoop::addConnection(std::unique_ptr<Connection> connection) {
	epoll_event event{};
	event.events = EPOLLIN;
	event.data.ptr = connection.get();
	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, connection->getFd(), &event) == -1) {
		std::cerr << "epoll_ctl failed for connection: " << strerror(errno) << "\n";
		return;
	}
	_connections.push_back(std::move(connection));
}

//still need to gracefully exit the loop when a signal is used.

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
			auto* socket = static_cast<Connection*>(events[i].data.ptr);
			socket->handleEvent(*this);
		}
		std::vector<int> timeoutFds;
		for (size_t i = 0; i < _connections.size(); i++) {
			if(time(nullptr) - _connections[i]->getLastActivity() > TIMEOUT_SECONDS) {
				std::cout << "connection timed out for fd: " <<_connections[i]->getFd() << std::endl;
				timeoutFds.push_back(_connections[i]->getFd());
			}
		}
		for (int fd : timeoutFds) {
			removeConnection(fd);
		}
	}
}

void EventLoop::removeConnection(int fd) {
	std::cout << "removing connection for fd: " << fd << std::endl;
	epoll_ctl(_epollFd, EPOLL_CTL_DEL, fd, nullptr);
	_connections.erase(
		std::remove_if(_connections.begin(), _connections.end(), 
			[fd](const std::unique_ptr<Connection> &s){
				return s->getFd() == fd;
			}),
		_connections.end());
}
