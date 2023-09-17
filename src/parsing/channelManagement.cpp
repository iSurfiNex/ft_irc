/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channelManagement.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 18:44:58 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/17 23:12:33 by rsterin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

void cmdKick(strVec_t &args, Client &origin, IrcServer &server)
{
	if (args.size() < 2)
		origin.msg(ERR_NEEDMOREPARAMS, "KICK");
	else
	{
		Channel *channel;
		channel = server.getChannelWithName(args[0]);
		if (!channel || !channel->isUserInside(origin))
			origin.msg(ERR_NOSUCHCHANNEL, args[0]);
		else if (!channel->isMod(origin))
			origin.msg(ERR_CHANOPRIVSNEEDED, channel->name);
		else
		{
			for (size_t i = 1; args.size() > i; i++)
			{
				Client *target;
				target = server.getClientWithNickname(args[i]);
				if (!target || !channel->isUserInside(*target))
					origin.msg(ERR_USERNOTINCHANNEL, args[i], channel->name);
				else
				{
					target->msg(MSG_PART, channel->name);
					channel->removeMod(*target);
					channel->removeUser(*target);
					std::string messageToOrigin = "You have successfully kicked " + target->nickname + " from the channel: " + channel->name + ".\r\n";
					std::cout << origin << " has kicked " << target << " from " << channel->name << std::endl;
					origin.sendMessage(messageToOrigin);
				}
			}
		}
	}
}

void cmdTopic(strVec_t &args, Client &origin, IrcServer &server)
{
	if (args.size() < 1)
		origin.msg(ERR_NEEDMOREPARAMS, "TOPIC");
	else
	{
		Channel *channel;
		channel = server.getChannelWithName(args[0]);
		if (!channel)
			origin.msg(ERR_NOSUCHCHANNEL, args[0]);
		else if (!channel->isUserInside(origin))
			origin.msg(ERR_NOTONCHANNEL, args[0]);
		else if (args.size() == 1)
		{
			if (channel->topic.empty())
				origin.msg(RPL_NOTOPIC, args[0]);
			else
				origin.msg(RPL_TOPIC, args[0], channel->topic);
		}
		else if (channel->isMod(origin) || channel->isTopicChangeable)
		{
			channel->changeTopic(args[1]);
			origin.msg(RPL_TOPIC, args[0], channel->topic);
		}
		else
			origin.msg(ERR_CHANOPRIVSNEEDED, channel->name);
	}
}

void cmdMode(strVec_t &args, Client &origin, IrcServer &server)
{
	if (args.size() < 2)
		return ;
	else
	{
		Channel *channel;
		channel = server.getChannelWithName(args[0]);

		if (!channel || !channel->isUserInside(origin))
			origin.msg(ERR_NOSUCHCHANNEL, args[0]);
		else if (!channel->isMod(origin))
			origin.msg(ERR_CHANOPRIVSNEEDED, channel->name);
		else if (!args[1].compare("+t") || !args[1].compare("-t"))
		{
			if (args[1][0] == '+')
				channel->isTopicChangeable = false;
			else if (args[1][0] == '-')
				channel->isTopicChangeable = true;
			origin.msg(MSG_MODE, "MODE", args[1]);
		}
		else if (!args[1].compare("+i") || !args[1].compare("-i"))
		{
			if (args[1][0] == '+')
				channel->isInviteOnly = true;
			else if (args[1][0] == '-')
				channel->isInviteOnly = false;
			origin.msg(MSG_MODE, channel->name, args[1]);
		}
		else if (!args[1].compare("+k") || !args[1].compare("-k"))
		{
			if (args[1][0] == '-')
			{
				channel->isRestricted = false;
				origin.msg(MSG_MODE, "MODE", args[1]);
			}
			else if (args.size() >= 3)
			{
				if (args[2].find_first_of(" \a\b\t\n\v\f\r,") != std::string::npos || args[2][0] == '\0' || args[2][0] == '#')
					origin.msg(ERR_NOTVALIDENTRY, channel->name, args[2]);
				else
				{
					channel->isRestricted = true;
					channel->changePassword(args[2]);
					origin.msg(MSG_MODE, "MODE", args[1]);
				}
			}
			else
				origin.msg(ERR_NEEDMOREPARAMS, "MODE");
		}
		else if (!args[1].compare("+l") || !args[1].compare("-l"))
		{
			if (args[1][0] == '-')
			{
				channel->changeUserLimit(-1);
				origin.msg(MSG_MODE, "MODE", args[1]);
			}
			else if (args.size() >= 3)
			{
				std::stringstream tmp(args[2]);
				int userLimit;

				if (args[2][0] == '\0' || !(tmp >> userLimit) || !tmp.eof())
					origin.msg(ERR_NOTVALIDENTRY, channel->name, args[2]);
				else if (userLimit < 0)
					origin.msg(ERR_NOTVALIDENTRY, channel->name, args[2]);
				else
				{
					channel->changeUserLimit(userLimit);
					origin.msg(MSG_MODE, "MODE", args[1]);
				}
			}
			else
				origin.msg(ERR_NEEDMOREPARAMS, "MODE");
		}
		else if (!args[1].compare("+o") || !args[1].compare("-o"))
		{
			if (args.size() >= 3)
			{
				Client *target;
				target = server.getClientWithNickname(args[2]);

				if (!target)
					origin.msg(ERR_NOSUCHNICK, args[2]);
				else if (!channel->isUserInside(*target))
					origin.msg(ERR_USERNOTINCHANNEL, args[2], channel->name);
				else if (args[1][0] == '+' || args[1][0] == '-')
				{
					if (args[1][0] == '+')
						channel->addMod(*target);
					else if (args[1][0] == '-')
						channel->removeMod(*target);
					origin.msg(MSG_MODE, "MODE", args[1]);
				}
			}
			else
				origin.msg(ERR_NEEDMOREPARAMS, "MODE");
		}
	}
}
