/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIProcess.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 12:56:01 by rmhazres          #+#    #+#             */
/*   Updated: 2026/07/10 17:25:11 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <csignal>
#include "../server_setup/ASocket.hpp"
#include "../Connection/Connection.hpp"
#include "../Connection/EventLoop.hpp"

class Connection;
class CGIProcess : public ASocket
{
	public:
		CGIProcess(int fd, pid_t pid, Connection& connection);
		~CGIProcess();
		void handleEvent(EventLoop &loop) override;
	private:
		pid_t _pid;
		std::string _output;
		Connection& _connection;
		
};