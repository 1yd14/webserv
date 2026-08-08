/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIProcess.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 12:56:01 by rmhazres          #+#    #+#             */
/*   Updated: 2026/08/08 14:58:54 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <csignal>
#include "../server_setup/ASocket.hpp"
#include "../Connection/EventLoop.hpp"
#include "../Config/Server.hpp"

class Connection;
class CGIProcess : public ASocket
{
	public:
		CGIProcess(int fd, pid_t pid, uint64_t connectionId, const Server& server);
		~CGIProcess();
		void handleEvent(EventLoop &loop) override;
	private:
		pid_t _pid;
		std::string _output;
		uint64_t _connectionId;
		const Server& _server; 
		
};