/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 19:55:29 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/15 18:49:13 by rsterin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"


const std::string Channel::_allowedNamePrefix = "&#+!";
const std::string Channel::_forbiddenNameChars = " \a,";
const int Channel::_nameMaxLen = 50;

Channel::Channel(const std::string &_name, const std::string &key, Client &mod, const IrcServer &server): _server(server)
{
	name = _name;

	isInviteOnly = false;
	isTopicChangeable = false;
	isRestricted = false;
	_userLimit = -1;
	_password = key;
	topic = "";
	_symbol = "=";
	_addUser(mod);
	addMod(mod);
}

Channel::~Channel(void)
{

}

void Channel::_addUser(const Client &client)
{
	_userList.insert(&client);
	msg(MSG_JOIN, client.nickname);
	if (!topic.empty())
		client.msg(RPL_TOPIC, name, topic);
	client.msg(RPL_NAMREPLY, _symbol, name, _getUserListStr());
	client.msg(RPL_ENDOFNAMES, name);
}

void Channel::tryEnter(Client &client, const std::string& key)
{
    if (isInviteOnly && !isUserOnInviteList(client))
      client.msg(ERR_INVITEONLYCHAN, name);
    else if (isRestricted && !checkPassword(key))
      client.msg(ERR_BADCHANNELKEY, name);
	else
	  _addUser(client);
}

std::string Channel::_getUserListStr(void) const
{
	std::string result;

    foreach(std::set<const Client *>, _userList) {
		const Client * client = *it;
        if (!result.empty()) {
            result += " ";
        }
        result += client->nickname;
    }

    return result;
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

void Channel::changeTopic(const std::string tmpTopic)
{
	topic = tmpTopic;
	std::cout << "Channel: " << name << ", has a new topic: " << topic << std::endl;
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

bool Channel::isMod(const Client &client) const
{
	return _modList.find(&client) != _modList.end();
}

bool Channel::isUserInside(const Client &client) const
{
	return _userList.find(&client) != _userList.end();
}

bool Channel::isUserOnInviteList(const Client &client) const
{
	return _inviteList.find(&client) != _inviteList.end();
}

bool Channel::checkPassword(const std::string &pw) const
{
	return pw == _password;
}

void Channel::sendMessage(const std::string message) const
{
	foreach(constClientSet_t, _userList)
	{
		const Client *client = *it;
		client->sendMessage(message);
	}
}

void Channel::msg(msgCode_e code, ...) const
{
	std::map<std::string, std::string> presets;
	presets["<code>"] = itoa(code);
	presets["<channel>"] = name;
	presets["<server>"] = _server.name;

	va_list args;
    va_start(args, code);

	std::string msgStr = IrcServer::formatCode(code, presets, args);
	std::cout << msgStr << std::endl;
	sendMessage(msgStr);
	va_end(args);
}

bool Channel::isValidKey(const std::string &key)
{
	return _isValidBaseName(key);
}

bool Channel::_isValidNamePrefix(char c)
{
	return _allowedNamePrefix.find(c) != std::string::npos;
}

bool Channel::_isValidBaseName(const std::string &name)
{
	return name.find_first_of(_forbiddenNameChars) == std::string::npos;
}

bool Channel::isValidName(const std::string &name)
{
    int channelNameSize = name.size();

	if (channelNameSize < 2)
		return false;

	if (channelNameSize > _nameMaxLen)
		return false;

    if (!_isValidNamePrefix(name[0]))
		return false;

    std::string baseName = name;
    baseName.erase(0, 1);
	if (!_isValidBaseName(baseName))
		return false;

	return true;
}
