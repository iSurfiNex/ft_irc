/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 19:46:41 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/11 19:55:59 by rsterin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "IrcServer.hpp"

class Client
{
	public:
		// Client(void);
		Client(const std::string username, const std::string nickname);
		~Client(void);

		std::string username;
		std::string nickname;
};
