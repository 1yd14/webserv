/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 16:53:32 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/07/07 15:45:45 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <memory>
#include <vector>
#include "../server_setup/ASocket.hpp"
#include <sys/epoll.h>
#include "../http/CGIProcess.hpp"

class Connection;
class CGIProcess;

class EventLoop {
	private:
	int _epollFd;
	std::vector<std::unique_ptr<ASocket>> _listeners;
	std::vector<std::unique_ptr<Connection>> _connections;
	std::vector<std::unique_ptr<CGIProcess>> _cgiProcesses;

	public:
	EventLoop();
	~EventLoop();
	void addListeningSocket(std::unique_ptr<ASocket> socket);
	void addConnection(std::unique_ptr<Connection> connection);
	void addCgi(std::unique_ptr<CGIProcess> cgiProcess);
	void run();
	void removeConnection(int fd);
	void removeCGIProcess(int fd);
	bool setReading(ASocket *socket, int op) const ;
	bool setWriting(ASocket *socket, int op) const ;
	bool isCGIProcess(int fd);
};
