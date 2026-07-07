/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 16:20:35 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/07/07 15:44:20 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../server_setup/ASocket.hpp"
#include "../Config/Server.hpp"
#include <ctime>
#include <string>

enum State { READING, WRITING, CLOSING };

class Connection : public ASocket {
	private:
	const Server&	_server;
	std::string		_readBuffer;
	std::string		_writeBuffer;
	State			_state;
	time_t			_lastActivity;
	
	public:
	Connection(int fd, const Server& server);
	~Connection();
	void setState(State newState);
	State getState();
	time_t getLastActivity() const;
	void handleEvent(EventLoop &loop) override;
	void handleRead(EventLoop &loop);
	void handleWrite(EventLoop &loop);
	void setWriterBuffer(const std::string& data);
};
