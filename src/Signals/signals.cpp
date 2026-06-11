/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyvan-de <lyvan-de@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 10:21:36 by lyvan-de          #+#    #+#             */
/*   Updated: 2026/06/10 10:38:46 by lyvan-de         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.hpp"
#include <iostream>

volatile sig_atomic_t g_sig_val = 0;

void signalHandler(int sigVal) {
	(void) sigVal;
	g_sig_val = 1;
}
