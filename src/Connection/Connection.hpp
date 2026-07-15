/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 16:20:35 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/07/15 14:51:50 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../server_setup/ASocket.hpp"
#include "../Config/Server.hpp"
#include "EventLoop.hpp"
#include <cstdint>
#include <ctime>
#include <string>

enum State { READING, WRITING, CLOSING , ERROR_PENDING};

class Connection : public ASocket {
	private:
	const Server&	_server;
	std::string		_readBuffer;
	std::string		_writeBuffer;
	State			_state;
	time_t			_lastActivity;
	std::string		_pendingError;
	bool			_shouldClose = false;
	uint64_t		_id;
	static uint64_t	s_nextId;
	
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
	bool isRequestComplete();
	void handleErrorPending(EventLoop& loop);
	std::string prepareRequest();
	void dispatch(const std::string& requestToParse, EventLoop& loop);
	uint64_t getId();
};
