/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 14:14:22 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/14 14:59:21 by rsterin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

Client::Client(int socketId, const IrcServer &_server): socketId(socketId), server(_server)
{
	isReady = false;
	isAuth = false;

	serverName = ":myserver";
	nickname = "";
	username = "";
}

Client::~Client(void)
{
	chanSet_t userChans = server.getUserChans(*this);
	foreach(chanSet_t, userChans)
	{
		Channel *chan = *it;
		chan->removeUser(*this);
		chan->removeUserFromInviteList(*this);
		chan->removeMod(*this);
	}
}

void Client::changeNickName(const std::string newNickname)
{
	nickname = newNickname;

	if (!(username.empty()) && !(nickname.empty()) && !isReady)
	{
		isReady = true;
		std::cout << "New user authentificate: " << *this << std::endl;
		sendMessage("You are now authentificate.\r\n");
	}
}

void Client::changeUserName(const std::string newUsername)
{
	username = newUsername;

	if (!(username.empty()) && !(nickname.empty()) && !isReady)
	{
		isReady = true;
		std::cout << "New user authentificate: " << *this << std::endl;
		sendMessage("You are now authentificate.\r\n");
	}
}

// TODO private
void Client::sendMessage(const std::string &message) const
{
	std::string msgStr = message; // TODO truncate 512 char
	send(socketId, msgStr.c_str(), msgStr.size(), 0);
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

void Client::msg(msgCode_e code, ...) const
{
	std::map<std::string, std::string> presets;
	presets["<code>"] = itoa(code);
	presets["<client>"] = nickname;
	presets["<server>"] = serverName;

	va_list args;
    va_start(args, code);

	std::string msgStr = IrcServer::formatCode(code, presets, args);
	std::cout << msgStr << std::endl;
	sendMessage(msgStr + "\r\n"); //TODO rename sendStr after rebase
	va_end(args);
}

std::ostream	&operator <<(std::ostream &os, const Client &client)
{
  os << "Client[id:" GREEN << client.socketId << NC ", nick:" PURPLE << client.nickname << NC ", username:" PURPLE
     << client.username << NC ", auth:" YELLOW
     << (client.isAuth ? "Y":"N") << NC ", ready:" YELLOW
     << (client.isReady ? "Y":"N") << NC "]";
  return os;
}
