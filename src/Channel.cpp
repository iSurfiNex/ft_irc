/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 19:55:29 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/14 18:16:31 by rsterin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

Channel::Channel(const std::string &cName)
{
	name = cName;

	std::cout << "New channel: " << name << std::endl;

	isInviteOnly = false;
	isTopicChangeable = false;
	isRestricted = false;
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
	msg += name;

	if (isUserOnInviteList(client))
	{
		removeUserFromInviteList(client);
	    std::cout << "New user" << client.getUsername() << " consumed invitation to join channel: " << name << std::endl;
		msg += " by consuming your invitation";
	}

	msg += ".\r\n";

	client.sendMessage(msg);
	std::cout << "New user: " << client.getUsername() << ", in channel: " << name << std::endl;
}

void Channel::addUserInviteList(const Client &client)
{
	_inviteList.insert(&client);

	std::string msg = "You have been invited to: ";
	msg += name;
	msg += ".\r\n";

	client.sendMessage(msg);
	std::cout << "New user: " << client.getUsername() << ", on channel's invite-list: " << name << std::endl;
}

void Channel::addMod(const Client &client)
{
	_modList.insert(&client);

	std::string msg = "You are now operator of: ";
	msg += name;
	msg += ".\r\n";

	client.sendMessage(msg);
	std::cout << "New mod: " << client.getUsername() << ", in channel: " << name << std::endl;
}

void Channel::removeUser(const Client &client)
{
	if (_userList.find(&client) != _userList.end())
	{
		_userList.erase(&client);

		std::string msg = "You have been remove of: ";
		msg += name;
		msg += ".\r\n";

		client.sendMessage(msg);
		std::cout << "User: " << client.getUsername() << ", has been removed of: " << name << std::endl;
	}
}

void Channel::removeUserFromInviteList(const Client &client)
{
	if (_inviteList.find(&client) != _userList.end())
	{
		_inviteList.erase(&client);

		std::string msg = "You have been remove of: ";
		msg += name;
		msg += ".\r\n";

		client.sendMessage(msg);
		std::cout << "User: " << client.getUsername() << ", has been removed of: " << name << std::endl;
	}
}

void Channel::removeMod(const Client &client)
{
	if (_modList.find(&client) != _modList.end())
	{
		_modList.erase(&client);

		std::string msg = "You have been remove of: ";
		msg += name;
		msg += " mod list.\r\n";

		client.sendMessage(msg);
		std::cout << "User: " << client.getUsername() << ", has been removed of: " << name << " mod list." << std::endl;
	}
}

void Channel::changeRule(const char mode)
{
	switch (mode)
	{
		case 'i':
		{
			isInviteOnly = !isInviteOnly;
			std::cout << "Channel: " << name << ", invite-only is now set to " << isInviteOnly << std::endl;
			break;
		}
		case 't':
		{
			isTopicChangeable = !isTopicChangeable;
			std::cout << "Channel: " << name << ", topic-changeable is now set to " << isInviteOnly << std::endl;
			break;
		}
		case 'k':
		{
			isRestricted = !isRestricted;
			std::cout << "Channel: " << name << ", restricted-mode is now set to " << isInviteOnly << std::endl;
			break;
		}

		default:
			break;
	}
}

void Channel::changeTopic(const std::string topic)
{
	_topic = topic;
	std::cout << "Channel: " << name << ", has a new topic: " << _topic << std::endl;
}

void Channel::changePassword(const std::string password)
{
	_password = password;
	std::cout << "Channel: " << name << ", has a new password: " << _password << std::endl;
}

void Channel::changeUserLimit(const int userlimit)
{
	_userLimit = userlimit;
	std::cout << "Channel: " << name << ", has a new user-limit: " << _userLimit << std::endl;
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

bool Channel::isUserOnInviteList(const Client &client)
{
	return _inviteList.find(&client) != _inviteList.end();
}

bool Channel::checkPassword(const std::string &pw)
{
	return pw == _password;
}

void Channel::sendMessage(const std::string message) const
{
	for (std::set<const Client *>::iterator it = _userList.begin(); _userList.end() != it; ++it)
	{
		const Client *client = *it;
		send(client->socketId, message.c_str(), message.size(), 0);
	}
}
