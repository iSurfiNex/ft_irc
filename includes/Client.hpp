/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 19:46:41 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/12 19:00:51 by rsterin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "IrcServer.hpp"

class Client
{
	public:
		Client(int socketId);
		~Client(void);

		void changeUserName(const std::string newUsername);
		void changeNickName(const std::string newNickname);

		void sendMessage(const std::string message) const;

		std::string getUsername(void) const;
		std::string getNickname(void) const;
		int getSocketId(void) const;

		bool isAuth;
		bool isReady;

		std::string username;
		std::string nickname;
		const int socketId;
};
