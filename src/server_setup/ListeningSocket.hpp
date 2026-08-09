/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 17:56:26 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/08/09 15:53:32 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ASocket.hpp"
#include "../Config/Server.hpp"

class EventLoop; 

class ListeningSocket : public ASocket {
	private:
	const Server& _server;
	const int& _port;

	public:
	ListeningSocket(const Server& server, const int& port);
	void bindSocket(int port);
	void listenSocket();
	void handleEvent(EventLoop &loop) override;
	void onTimeout(EventLoop &loop) override;
};
