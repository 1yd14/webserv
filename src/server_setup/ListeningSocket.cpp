/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 19:45:49 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/06/02 11:09:20 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ListeningSocket.hpp"
#include <stdexcept>
#include <netinet/in.h>
#include <sys/socket.h>

ListeningSocket::ListeningSocket(const Server& server) :
		ASocket(socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)), _server(server) {
	if (getFd() == -1) {
		throw std::runtime_error("socket fd is not created");
	}
	int opt = 1;
	setsockopt(getFd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	
}


