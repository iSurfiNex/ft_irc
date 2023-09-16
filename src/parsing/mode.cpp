/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 18:49:53 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/16 16:30:11 by rsterin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

void cmdMode(strVec_t &args, Client &origin, IrcServer &server)
{
	if (args.size() < 2)
		origin.msg(ERR_NEEDMOREPARAMS, "MODE");
	else
	{
		Channel *channel;
		channel = server.getChannelWithName(args[0]);

		if (!channel || !channel->isUserInside(origin))
			origin.msg(ERR_NOSUCHCHANNEL, args[0]);
		else if (channel->isMod(origin))
			origin.msg(ERR_CHANOPRIVSNEEDED, channel->name);
		else if (args[1].compare("+t") || args[1].compare("-t"))
		{
			if (args[1][0] == '+')
				channel->isTopicChangeable = false;
			else if (args[1][0] == '-')
				channel->isTopicChangeable = true;
			origin.msg(MSG_RULECHANGED, channel->name, "TOPIC");
		}
		else if (args[1].compare("+i") || args[1].compare("-i"))
		{
			if (args[1][0] == '+')
				channel->isInviteOnly = true;
			else if (args[1][0] == '-')
				channel->isInviteOnly = false;
			origin.msg(MSG_RULECHANGED, channel->name, "INVITE-ONLY");
		}
		else if (args[1].compare("+k") || args[1].compare("-k"))
		{
			if (args[1][0] == '-')
				channel->isRestricted = false;
			else if (args.size() < 3)
			{
				if (args[2].find_first_of(" \a\b\t\n\v\f\r,") != std::string::npos || args[2][0] == '\0' || args[2][0] == '#')
					origin.msg(ERR_NOTVALIDENTRY, channel->name, args[2]);
				else
				{
					channel->isRestricted = true;
					channel->changePassword(args[2]);
				}
			}
			else if (args[1][0] == '+')
				origin.msg(ERR_NEEDMOREPARAMS, "MODE");
			else
				origin.msg(MSG_RULECHANGED, channel->name, "PASSWORD");
		}
		else if (args[1].compare("+l") || args[1].compare("-l"))
		{
			if (args[1][0] == '-')
				channel->changeUserLimit(-1);
			if (args.size() < 3)
			{
				std::stringstream tmp(args[2]);
				int userLimit;

				if (args[2][0] == '\0' || !(tmp >> userLimit) || !tmp.eof())
					origin.msg(ERR_NOTVALIDENTRY, channel->name, args[2]);
				else if (userLimit < 0)
					origin.msg(ERR_NOTVALIDENTRY, channel->name, args[2]);
				else
					channel->changeUserLimit(userLimit);
			}
			else if (args[1][0] == '+')
				origin.msg(ERR_NEEDMOREPARAMS, "MODE");
			else
				origin.msg(MSG_RULECHANGED, channel->name, "USER-LIMIT");
		}
		else if (args[1].compare("+o") || args[1].compare("-o"))
		{
			if (args.size() < 3)
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
					origin.msg(MSG_RULECHANGED, channel->name, "OPERATOR");
				}
			}
			else
				origin.msg(ERR_NEEDMOREPARAMS, "MODE");
		}
	}
}
