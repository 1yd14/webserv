/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpPipeline.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 14:37:11 by rmhazres          #+#    #+#             */
/*   Updated: 2026/08/12 12:36:41 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../Common/HttpRequest.hpp"
#include "../Config/Server.hpp"

std::string processRequest(const HttpRequest& request, const Server& server);
