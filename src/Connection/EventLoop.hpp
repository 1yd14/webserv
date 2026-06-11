/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 16:53:32 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/06/11 11:35:08 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <memory>
#include <vector>
#include "../server_setup/ASocket.hpp"
#include <sys/epoll.h>

class Connection;

class EventLoop {
	private:
	int _epollFd;
	std::vector<std::unique_ptr<ASocket>> _listeners;
	std::vector<std::unique_ptr<Connection>> _connections;

	public:
	EventLoop();
	~EventLoop();
	void addListeningSocket(std::unique_ptr<ASocket> socket);
	void addConnection(std::unique_ptr<Connection> connection);
	void run();
	void removeConnection(int fd);
	bool setReading(ASocket *socket, int op) const ;
	bool setWriting(ASocket *socket, int op) const ;
};
