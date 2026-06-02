/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponseBuilder.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmhazres <rmhazres@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 16:15:15 by rmhazres          #+#    #+#             */
/*   Updated: 2026/06/02 16:32:01 by rmhazres         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../common/HttpResponse.hpp"

class HttpResponseBuilder
{
	public:
			HttpResponseBuilder();
			~HttpResponseBuilder();
			HttpResponseBuilder(const HttpResponseBuilder &other) = delete;
			HttpResponseBuilder& operator=(const HttpResponseBuilder &other) = delete;			
			HttpResponse& build();
	
	private:
	
};