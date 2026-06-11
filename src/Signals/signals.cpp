/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 10:21:36 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/06/11 16:10:42 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.hpp"

volatile sig_atomic_t g_sig_val = 0;

void signalHandler(int sigVal) {
	(void) sigVal;
	g_sig_val = 1;
}
