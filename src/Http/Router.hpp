/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 12:42:30 by rmhazres          #+#    #+#             */
/*   Updated: 2026/08/12 12:36:41 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../Common/RouteType.hpp"
#include "../Config/Server.hpp"
#include "../Common/HttpRequest.hpp"

class Router
{
	public:
						Router() = default;
						~Router() = default;
						Router(const Router &other) = delete;
						Router& operator=(const Router &other) = delete;
	[[nodiscard]]		RouteType route(const HttpRequest& request, const Server &server) const;
	private:
};