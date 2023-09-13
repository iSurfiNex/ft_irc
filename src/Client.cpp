/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 14:14:22 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/13 20:15:06 by rsterin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

Client::Client(int socketId): socketId(socketId)
{
	isReady = false;
	isAuth = false;
}

Client::~Client(void)
{

}

void Client::changeNickName(const std::string newNickname)
{
	nickname = newNickname;

	if (!(username.empty()) && !(nickname.empty()) && !isReady)
	{
		std::cout << "New user: " << nickname << " (" << username << ")" << std::endl;
		sendMessage("You are now authentificate.\r\n");
		isReady = true;
	}
}

void Client::changeUserName(const std::string newUsername)
{
	username = newUsername;

	if (!(username.empty()) && !(nickname.empty()) && !isReady)
	{
		std::cout << "New user: " << nickname << " (" << username << ")" << std::endl;
		sendMessage("You are now authentificate.\r\n");
		isReady = true;
	}
}

void Client::sendMessage(const std::string message) const
{
	send(socketId, message.c_str(), message.size(), 0);
}

std::string Client::getUsername(void) const
{
	return (username);
}

std::string Client::getNickname(void) const
{
	return (username);
}

int Client::getSocketId(void) const
{
	return (socketId);
}
