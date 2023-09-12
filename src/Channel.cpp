/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 19:55:29 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/12 15:21:00 by rsterin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(const std::string cName)
{
	channelname = cName;

	std::cout << "New channel: " << channelname << std::endl;

	_isInviteOnly = false;
	_isTopicChangeable = false;
	_isRestricted = false;
	_userLimit = -1;
	_password = "";
	_topic = "";
}

Channel::~Channel(void)
{

}

void Channel::addUser(const Client &client)
{
	_userList.insert(client);

	std::string msg = "You have join: ";
	msg += channelname;
	msg += ".";

	client.sendMessage(msg);
	std::cout << "New user: " << client.getUsername() << " in channel: " << channelname << std::endl;
}

void Channel::addMod(const Client &client)
{
	_modList.insert(client);

	std::string msg = "You are now operator of: ";
	msg += channelname;
	msg += ".";

	client.sendMessage(msg);
	std::cout << "New mod: " << client.getUsername() << " in channel: " << channelname << std::endl;
}

void Channel::removeUser(const Client &client)
{
	if (_userList.find(client) != _userList.end())
	{
		_userList.erase(client);

		std::string msg = "You have been remove of: ";
		msg += channelname;
		msg += ".";

		client.sendMessage(msg);
		std::cout << "User: " << client.getUsername() << " has been removed of: " << channelname << std::endl;
	}
}

void Channel::removeMod(const Client &client)
{
	if (_modList.find(client) != _modList.end())
	{
		_modList.erase(client);

		std::string msg = "You have been remove of: ";
		msg += channelname;
		msg += " mod list.";

		client.sendMessage(msg);
		std::cout << "User: " << client.getUsername() << " has been removed of: " << channelname << " mod list." << std::endl;
	}
}
