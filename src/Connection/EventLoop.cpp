/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 16:53:28 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/06/09 18:49:25 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EventLoop.hpp"
#include <algorithm>
#include <cstring>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <sys/epoll.h>
#include <csignal>
#include <array>

#define MAX_EVENTS 1024

EventLoop::EventLoop(){
	_epollFd = epoll_create1(0);
	if (_epollFd == -1) {
		throw std::runtime_error("epoll cannot be created");
	}
}

// this function throws an exception because these ones are added before creating up epoll loop
void EventLoop::addListeningSocket(std::unique_ptr<ASocket> socket) {
	epoll_event event{};
	event.events = EPOLLIN;
	event.data.ptr = socket.get();
	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, socket->getFd(), &event) == -1) {
		throw std::runtime_error("epoll_ctl failed: " + std::string(strerror(errno)));
	}
	_sockets.push_back(std::move(socket));
}

// this function is similar to addListeningSocket but does not throw an exception because of time sensitivity
void EventLoop::addConnection(std::unique_ptr<ASocket> socket) {
	epoll_event event{};
	event.events = EPOLLIN;
	event.data.ptr = socket.get();
	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, socket->getFd(), &event) == -1) {
		std::cerr << "epoll_ctl failed for connection: " << strerror(errno) << "\n";
		return;
	}
	_sockets.push_back(std::move(socket));
}

//still need to gracefully exit the loop when a signal is used.

void EventLoop::run() {
	std::array<epoll_event, MAX_EVENTS> events;
	sigset_t	mask;
	sigemptyset(&mask);
	sigaddset(&mask, SIGINT);
	sigaddset(&mask, SIGTERM);

	while(true) {
		int readyFds = epoll_pwait(_epollFd, events.data(), events.size(), -1, &mask);
		if (readyFds < 0) {
			//hanlde the error
			continue;
		}
		for (int i = 0; i < readyFds; ++i)
		{
			auto* socket = static_cast<ASocket*>(events[i].data.ptr);
			socket->handleEvent(*this);
		}
	}
}

void EventLoop::removeConnection(int fd) {
	epoll_ctl(_epollFd, EPOLL_CTL_DEL, fd, nullptr);
	_sockets.erase(
		std::remove_if(_sockets.begin(), _sockets.end(), 
			[fd](const std::unique_ptr<ASocket> &s){
				return s->getFd() == fd;
			}),
		_sockets.end());
}
