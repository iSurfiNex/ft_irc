/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 19:55:29 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/12 14:40:20 by rsterin          ###   ########.fr       */
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

void Channel::addUser(const std::string username)
{
	userList.insert(username);
}

void Channel::addMod(const std::string username)
{
	modList.insert(username);
}

void Channel::removeUser(const std::string username)
{
	userList.erase(username);
}

void Channel::removeMod(const std::string username)
{
	if (modList.find(username) != modList.end())
		modList.erase(username);
}
