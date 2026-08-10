/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpPipeline.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 14:37:11 by rmhazres          #+#    #+#             */
/*   Updated: 2026/08/10 15:18:49 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../common/HttpRequest.hpp"
#include "../Config/Server.hpp"

std::string processRequest(const HttpRequest& request, const Server& server);

