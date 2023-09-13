/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 19:55:29 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/12 18:13:43 by rsterin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

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
	_userList.insert(&client);

	std::string msg = "You have join: ";
	msg += channelname;
	msg += ".\r\n";

	client.sendMessage(msg);
	std::cout << "New user: " << client.getUsername() << ", in channel: " << channelname << std::endl;
}

void Channel::addMod(const Client &client)
{
	_modList.insert(&client);

	std::string msg = "You are now operator of: ";
	msg += channelname;
	msg += ".\r\n";

	client.sendMessage(msg);
	std::cout << "New mod: " << client.getUsername() << ", in channel: " << channelname << std::endl;
}

void Channel::removeUser(const Client &client)
{
	if (_userList.find(&client) != _userList.end())
	{
		_userList.erase(&client);

		std::string msg = "You have been remove of: ";
		msg += channelname;
		msg += ".\r\n";

		client.sendMessage(msg);
		std::cout << "User: " << client.getUsername() << ", has been removed of: " << channelname << std::endl;
	}
}

void Channel::removeMod(const Client &client)
{
	if (_modList.find(&client) != _modList.end())
	{
		_modList.erase(&client);

		std::string msg = "You have been remove of: ";
		msg += channelname;
		msg += " mod list.\r\n";

		client.sendMessage(msg);
		std::cout << "User: " << client.getUsername() << ", has been removed of: " << channelname << " mod list." << std::endl;
	}
}

void Channel::changeRule(const char mode)
{
	switch (mode)
	{
		case 'i':
		{
			_isInviteOnly = !_isInviteOnly;
			std::cout << "Channel: " << channelname << ", invite-only is now set to " << _isInviteOnly << std::endl;
			break;
		}
		case 't':
		{
			_isTopicChangeable = !_isTopicChangeable;
			std::cout << "Channel: " << channelname << ", topic-changeable is now set to " << _isInviteOnly << std::endl;
			break;
		}
		case 'k':
		{
			_isRestricted = !_isRestricted;
			std::cout << "Channel: " << channelname << ", restricted-mode is now set to " << _isInviteOnly << std::endl;
			break;
		}

		default:
			break;
	}
}

void Channel::changeTopic(const std::string topic)
{
	_topic = topic;
	std::cout << "Channel: " << channelname << ", has a new topic: " << _topic << std::endl;
}

void Channel::changePassword(const std::string password)
{
	_password = password;
	std::cout << "Channel: " << channelname << ", has a new password: " << _password << std::endl;
}

void Channel::changeUserLimit(const int userlimit)
{
	_userLimit = userlimit;
	std::cout << "Channel: " << channelname << ", has a new user-limit: " << _userLimit << std::endl;
}

bool Channel::isMod(const Client &client)
{
	if (_modList.find(&client) != _modList.end())
		return (true);
	return (false);
}

bool Channel::isUserInside(const Client &client)
{
	if (_userList.find(&client) != _userList.end())
		return (true);
	return (false);
}
