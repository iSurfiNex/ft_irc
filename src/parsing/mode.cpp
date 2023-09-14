/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 18:49:53 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/14 19:41:47 by rsterin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

std::string cmdMode(std::vector<std::string> &args, Client &origin, IrcServer &server)
{
	if (args.size() != 2 || args.size() != 3)
		return ("Wrong number of arguments. Usage: MODE <channel> <param> (<msg>).\r\n");

	Channel *channel;
	channel = server.getChannelWithName(args[0]);
	if (!channel || !channel->isUserInside(origin))
		return ("Channel not found. Please enter a valid channel using: MODE <nickname> <channel>.\r\n");

	if (channel->isMod(origin))
		return ("You are not a mod of the channel.\r\n");

	if (args[1].compare("k"))
	{
		if (args.size() == 2)
			return ("Password restriction has been remove for " + channel->name + ".\r\n");
		else if (args.size() == 3)
		{
			if (args[2].find_first_of(" \a\b\t\n\v\f\r,") != std::string::npos || args[2][0] == '\0' || args[2][0] == '#')
				return ("Password is invalid.\r\n");
			channel->changePassword(args[2]);
			return ("Password has been set for " + channel->name + ".\r\n");
		}
		else
			return ("Wrong number of arguments. Usage: MODE <channel> <param> (<msg>).\r\n");
	}
	else if (args[1].compare("l"))
	{
		if (args.size() == 2)
		{
			channel->changeUserLimit(-1);
			return ("UserLimit restriction has been remove for " + channel->name + ".\r\n");
		}
		else if (args.size() == 3)
		{
			std::stringstream tmp(args[2]);
			int userLimit;

			if (args[2][0] == '\0' || !(tmp >> userLimit) || !tmp.eof())
				return ("Please enter a number. Usage: MODE <channel> <param> (<msg>).\r\n");
			if (userLimit < 0)
				return ("Please valid a number. Usage: MODE <channel> <param> (<msg>).\r\n");

			channel->changeUserLimit(userLimit);
			return ("UserLimit has been set for " + channel->name + ".\r\n");
		}
		else
			return ("Wrong number of arguments. Usage: MODE <channel> <param> (<msg>).\r\n");
	}
	else if (args[1].compare("o"))
	{
		if (args.size() != 3)
			return ("Wrong number of arguments. Usage: MODE <channel> <param> (<msg>).\r\n");

		Client *target;
		target = server.getClientWithNickname(args[2]);
		if (!target)
			return ("User not found . Please enter a valid target using: MODE <channel> <param> (<msg>).\r\n");

		if (!channel->isUserInside(*target))
			return ("User is not in targeted channel.\r\n");

		if (channel->isMod(*target))
		{
			channel->removeMod(*target);
			return (target->nickname + " has been removed from mod-list " + channel->name + ".\r\n");
		}
		else
		{
			channel->addMod(*target);
			return (target->nickname + "is now a mod of " + channel->name + ".\r\n");
		}
	}
	channel->changeRule(args[1][0]);

	return ("");
}
