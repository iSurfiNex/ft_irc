/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 14:14:22 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/12 17:50:52 by rsterin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

Client::Client(int socketId): _socketId(socketId)
{
	isReady = false;
}

Client::~Client(void)
{

}

void Client::changeNickName(const std::string newNickname)
{
	_nickname = newNickname;

	if (!(_username.empty()) && !(_nickname.empty()) && !isReady)
	{
		std::cout << "New user: " << _nickname << "(" << _username << ")" << std::endl;
		isReady = true;
	}
}

void Client::changeUserName(const std::string newUsername)
{
	_username = newUsername;

	if (!(_username.empty()) && !(_nickname.empty()) && !isReady)
	{
		std::cout << "New user: " << _nickname << "(" << _username << ")" << std::endl;
		isReady = true;
	}
}

void Client::sendMessage(const std::string message) const
{
	send(_socketId, message.c_str(), message.size(), 0);
}

std::string Client::getUsername(void) const
{
	return (_username);
}

std::string Client::getNickname(void) const
{
	return (_nickname);
}

int Client::getSocketId(void) const
{
	return (_socketId);
}
