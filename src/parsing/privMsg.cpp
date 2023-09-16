/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privMsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 16:58:52 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/16 17:27:04 by rsterin          ###   ########.fr       */
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
			origin.msg(ERR_NOSUCHNICK, args[0]);
		else
		{
			std::string messageToChannel = ":" + origin.nickname + " PRIVMSG " + channel->name + " :" + args[1];
			std::cout << origin << " send: \"" << messageToChannel << "\" to " << channel->name << std::endl;
			messageToChannel += "\r\n";
			channel->sendMessageIgnore(messageToChannel, origin);
		}
	}
	else
	{
		Client *target;
		target = server.getClientWithNickname(args[0]);
		if (!target)
			origin.msg(ERR_NOSUCHNICK, args[0]);
		else
		{
			std::string messageToTarget = ":" + origin.nickname + " PRIVMSG " + target->nickname + " :" + args[1];
			std::cout << origin << " send: \"" << messageToTarget << "\" to " << *target << std::endl;
			messageToTarget += "\r\n";
			target->sendMessage(messageToTarget);
		}
	}
}
