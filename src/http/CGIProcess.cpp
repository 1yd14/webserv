/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIProcess.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 13:05:09 by rmhazres          #+#    #+#             */
/*   Updated: 2026/08/08 15:58:17 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include <unistd.h>
#include "./CGIProcess.hpp"
#include "../common/HttpResponse.hpp"
#include "CGiHandler.hpp"
#include <array>

CGIProcess::CGIProcess(int fd, pid_t pid, uint64_t connectionId, const Server& server) : ASocket(fd), _pid(pid), _connectionId(connectionId), _server(server)
{
}

CGIProcess::~CGIProcess(){}

void CGIProcess::handleEvent(EventLoop &loop)
{
	std::array<char, 4096> buffer;
	ssize_t state = read(getFd(), buffer.data(), sizeof(buffer));
	Connection* conn = loop.getConnection(_connectionId);
	int status;
	
	if (state == -1)
	{
		waitpid(_pid, &status,0);
		if (conn != nullptr) {
			if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
			{
				std::string errorResponse = CGIHanlder::buildError(HttpStatus::INTERNAL_SERVER_ERROR, _server).serialize();
				conn->setWriterBuffer(errorResponse);
				conn->setState(WRITING);
				loop.setWriting(conn, EPOLL_CTL_MOD);
			}
		}
		loop.removeCGIProcess(getFd());
		return;
	}
	if (state == 0)
	{
		waitpid(_pid, &status,0);
		if (conn != nullptr) {
			HttpResponse response;
			if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
			{
				response = CGIHanlder::buildError(HttpStatus::INTERNAL_SERVER_ERROR, _server);
			}
			else
			{
				CGIHanlder::parseCGIOutput(_output, response, _server);
			}
			conn->setWriterBuffer(response.serialize());
			conn->setState(WRITING);
			loop.setWriting(conn, EPOLL_CTL_MOD);
		}
		loop.removeCGIProcess(getFd());
		return;
	}
	_output.append(buffer.data(), state);
}