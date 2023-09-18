/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverMsg.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 16:58:52 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/18 18:15:07 by rsterin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

void cmdPrivMsg(strVec_t &args, Client &origin, IrcServer &server)
{
	if (args.size() == 1)
		origin.msg(ERR_NOTEXTTOSEND);
	else if (args.size() != 2)
		origin.msg(ERR_NEEDMOREPARAMS, "PRIVMSG");
	else if (args[0][0] == '#')
	{
		Channel *channel;
		channel = server.getChannelWithName(args[0]);
		if (!channel || !channel->isUserInside(origin))
			origin.msg(ERR_NOSUCHNICK, &args[0]);
		else
		{
			std::string messageToChannel = ":" + origin.nickname + " PRIVMSG " + channel->name + " :" + args[1] + "\r\n";
			channel->sendMessageIgnore(messageToChannel, origin);
		}
	}
	else
	{
		Client *target;
		target = server.getClientWithNickname(args[0]);
		if (!target)
			origin.msg(ERR_NOSUCHNICK, &args[0]);
		else
		{
			std::string messageToTarget = ":" + origin.nickname + " PRIVMSG " + target->nickname + " :" + args[1] + "\r\n";
			target->sendMessage(messageToTarget);
		}
	}
}
