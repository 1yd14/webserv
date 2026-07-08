/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIProcess.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 13:05:09 by rmhazres          #+#    #+#             */
/*   Updated: 2026/07/08 16:21:34 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include <unistd.h>
#include "./CGIProcess.hpp"
#include "../common/HttpResponse.hpp"
#include "CGiHandler.hpp"
#include <array>
#include <iostream>
CGIProcess::CGIProcess(int fd, pid_t pid, Connection& connection) : ASocket(fd), _pid(pid), _connection(connection)
{
}

CGIProcess::~CGIProcess(){}

void CGIProcess::handleEvent(EventLoop &loop)
{
	std::array<char, 4096> buffer;
	ssize_t state = read(getFd(), buffer.data(), sizeof(buffer));
	
	if (state == -1)
	{
		std::string errorResponse = CGIHanlder::buildError(HttpStatus::INTERNAL_SERVER_ERROR).serialize();
		_connection.setWriterBuffer(errorResponse);
		_connection.setState(WRITING);
		loop.removeCGIProcess(getFd());
		return;
	}
	if ( state == 0)
	{
		HttpResponse response;
		waitpid(_pid, nullptr,0);
		CGIHanlder::parseCGIOutput(_output, response);
		_connection.setWriterBuffer(response.serialize());
		_connection.setState(WRITING);
		loop.setWriting(&_connection, EPOLL_CTL_MOD);
		loop.removeCGIProcess(getFd());
		return;
	}
	_output.append(buffer.data(), state);
}