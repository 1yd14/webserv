/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 17:56:26 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/06/09 18:48:16 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ASocket.hpp"
#include "../Config/Server.hpp"

class EventLoop; 

class ListeningSocket : public ASocket {
	private:
	const Server& _server;

	public:
	ListeningSocket(const Server& server);
	void bindSocket();
	void listenSocket();
	void handleEvent(EventLoop &loop) override;
};
