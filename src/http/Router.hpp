/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 12:42:30 by rmhazres          #+#    #+#             */
/*   Updated: 2026/06/03 12:51:25 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../common/RouteType.hpp"
#include "../Config/Server.hpp"
#include "../common/HttpRequest.hpp"

class Router
{
	public:
						Router() = default;
						~Router() = default;
						Router(const Router &other) = delete;
						Router& operator=(const Router &other) = delete;
[[nodiscard]]			RouteType route(const HttpRequest& request, const Server &server) const;
	private:
};