/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIProcess.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 12:56:01 by rmhazres          #+#    #+#             */
/*   Updated: 2026/07/14 16:39:06 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <csignal>
#include "../server_setup/ASocket.hpp"
#include "../Connection/EventLoop.hpp"

class Connection;
class CGIProcess : public ASocket
{
	public:
		CGIProcess(int fd, pid_t pid, int connectionFd);
		~CGIProcess();
		void handleEvent(EventLoop &loop) override;
	private:
		pid_t _pid;
		std::string _output;
		int _connectionFd;
		
};