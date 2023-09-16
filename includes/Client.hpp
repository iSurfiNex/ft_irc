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
# include <cstdarg>

class Client
{
	public:

		/* LIFECYCLE */

		Client(int socketId, struct sockaddr_in address, const IrcServer &server);
		~Client(void);

		/* ACTIONS */

		void sendMessage(const std::string &message) const;
		void msg(msgCode_e code, ...) const;

		/* GET SET */

		std::string getUsername(void) const;
		void changeUserName(const std::string newUsername);

		std::string getNickname(void) const;
		void changeNickName(const std::string newNickname);

		int getSocketId(void) const;

		/* ATTRIBUTES */

		bool isAuth;
		bool isReady;
		std::string username;
		std::string nickname;
		const int socketId;
		std::string partialMsg;
		struct sockaddr_in address;
		socklen_t addrlen;
		const IrcServer &_server;
		int maxChans;
};

std::ostream &operator<<(std::ostream &os, const Client &client);
