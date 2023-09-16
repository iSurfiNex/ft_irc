/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 18:38:10 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/16 19:07:13 by rsterin          ###   ########.fr       */
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
					channel->removeUser(*target);
					std::string messageToOrigin = "You have successfully kicked " + target->nickname + " from the channel: " + channel->name + ".\r\n";
					std::cout << origin << " has kicked " << target << " from " << channel->name << std::endl;
					origin.sendMessage(messageToOrigin);
				}
			}
		}
	}
}
