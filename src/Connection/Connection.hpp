/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 16:20:35 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/06/05 17:30:46 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../server_setup/ASocket.hpp"
#include "../Config/Server.hpp"

enum State { READING, WRITING, CLOSING };

class Connection : public ASocket {
	private:
	const Server&	_server;
	std::string		_readBuffer;
	std::string		_writeBuffer;
	State			_state;
	
	public:
	Connection(int fd, const Server& server);
	void setState(State newState);
	State getState();
	void handleEvent() const override;
};
