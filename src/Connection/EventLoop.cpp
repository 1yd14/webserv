/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 16:53:28 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/08/12 11:30:05 by lyvan-de         ###   ########.fr       */
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
#define TIMEOUT_SECONDS 30

EventLoop::EventLoop() {
	_epollFd = epoll_create1(0);
	if (_epollFd == -1) {
		throw std::runtime_error("epoll cannot be created");
	}
}

EventLoop::~EventLoop() {
	close(_epollFd);
}

void EventLoop::addListeningSocket(std::unique_ptr<ASocket> socket) {
	if (!setReading(socket.get(), EPOLL_CTL_ADD)) {
		throw std::runtime_error("epoll_ctl failed: " + std::string(strerror(errno)));
	}
	_listeners.push_back(std::move(socket));
}

void EventLoop::addConnection(std::unique_ptr<Connection> connection) {
	if (!setReading(connection.get(), EPOLL_CTL_ADD)) {
		std::cerr << "epoll_ctl failed for connection: " << strerror(errno) << "\n";
		return;
	}
	_connections.push_back(std::move(connection));
}

void EventLoop::addCgi(std::unique_ptr<CGIProcess> cgiProcess) {
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
        		continue;
			}
    		std::cerr << "epoll_wait: " << strerror(errno) << std::endl;
    		break;
		}
		for (int i = 0; i < readyFds; ++i) {
			auto* socket = static_cast<ASocket*>(events[i].data.ptr);
			if ((events[i].events & (EPOLLHUP | EPOLLERR | EPOLLRDHUP)) != 0) {
				if (isCGIProcess(socket->getFd()))
				{
					socket->handleEvent(*this);
					continue;
				}
				removeConnection(socket->getFd());
				continue;
			}
			socket->handleEvent(*this);
		}
		std::vector<ASocket*> timedOut;
		for (auto& conn : _connections) {
			if (time(nullptr) - conn->getLastActivity() > TIMEOUT_SECONDS){
				if (conn->getState() == AWAITING_CGI) {
					continue ;
				}
	        	timedOut.push_back(conn.get());
			}
		}
		for (auto& CGI : _cgiProcesses) {
			if(time(nullptr) - CGI->getLastActivity() > TIMEOUT_SECONDS) {
				timedOut.push_back(CGI.get());
			}
		}
		for (ASocket* socket : timedOut) {
    		socket->onTimeout(*this);
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
	epoll_ctl(_epollFd, EPOLL_CTL_DEL, fd, nullptr);
	_cgiProcesses.erase(
		std::remove_if(_cgiProcesses.begin(), _cgiProcesses.end(), 
			[fd](const std::unique_ptr<CGIProcess> &s){
				return s->getFd() == fd;
			}),
		_cgiProcesses.end());
}

Connection *EventLoop::getConnection(uint64_t id) {
	for (auto &c : _connections) {
		if (c->getId() == id) {
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
