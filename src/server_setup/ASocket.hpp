/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASocket.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 16:51:29 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/06/09 18:48:08 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// maybe rename to sockethandler, since this class is the base of the listner and the connections?
class EventLoop; 

class ASocket {
	private:
	int _fd;
	
	public:
	ASocket(int fd);
	int getFd() const;
	ASocket(const ASocket&)            = delete;
	ASocket& operator=(const ASocket&) = delete;
	virtual void handleEvent(EventLoop &loop) = 0;
	virtual ~ASocket();
};
