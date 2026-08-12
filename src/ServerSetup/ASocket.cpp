/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASocket.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 16:53:44 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/06/01 17:54:42 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ASocket.hpp"
#include <unistd.h>

ASocket::ASocket(int fd) : _fd(fd) {}

int ASocket::getFd() const {
	return (_fd);
}

ASocket::~ASocket() {
	if (_fd != -1) {
		close(_fd);
	}
}
