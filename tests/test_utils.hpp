/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_utils.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 11:20:47 by rmhazres          #+#    #+#             */
/*   Updated: 2026/05/22 11:21:21 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <iostream>

void assert_equal_str(const std::string &expected, const std::string &actual, const std::string &label)
{
	if(expected == actual)
	{
		std::cout << "[PASSED] " << label << "\n";		
	}
	else 
	{
		std::cout << "[FAILED] " << label << " - expected: " << expected << " 'got: " << actual <<"'\n";
	}
}

void assert_equal_int(int expected, int actual, const std::string &label)
{
	if(expected == actual)
	{
		std::cout << "[PASSED] " << label << "\n";		
	}
	else 
	{
		std::cout << "[FAILED] " << label << " - expected: " << expected << " 'got: " << actual <<"'\n";
	}
}
