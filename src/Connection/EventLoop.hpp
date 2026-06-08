/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 16:53:32 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/06/08 12:08:08 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <memory>
#include <unordered_map>
#include <vector>
#include "../server_setup/ASocket.hpp"
#include "Connection.hpp"
#include <poll.h>

class EventLoop {
	private:
	std::vector<pollfd> _epollFds;
	std::vector

	public:
	void addSocket(std::unique_ptr<ASocket> socket);
	void removeSocket(int fd);
	
};
