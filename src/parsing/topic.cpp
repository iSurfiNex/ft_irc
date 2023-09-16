/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 18:20:31 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/15 19:07:50 by rsterin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

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
