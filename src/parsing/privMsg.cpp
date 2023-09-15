/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privMsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 16:58:52 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/14 17:21:47 by rsterin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

std::string cmdPrivMsg(strVec_t &args, Client &origin, IrcServer &server)
{
	if (args.size() != 2)
		return ("Wrong number of arguments. Usage: PRIVMSG <channel/nickname> :<msg>.\r\n");

	if (args[0][0] == '#')
	{
		Channel *channel;
		channel = server.getChannelWithName(args[0]);
		if (!channel || !channel->isUserInside(origin))
			return ("Channel not found. Please enter a valid channel using: PRIVMSG <channel/nickname> :<msg>.\r\n");

		std::string messageToChannel = ":" + origin.nickname + " PRIVMSG " + channel->name + " :" + args[1];
		std::cout << origin << " send: \"" << messageToChannel << "\" to " << channel->name << std::endl;
		messageToChannel += "\r\n";
		channel->sendMessage(messageToChannel);
	}
	else
	{
		Client *target;
		target = server.getClientWithNickname(args[0]);
		if (!target)
			return ("User not found. Please enter a valid user using: PRIVMSG <channel/nickname> :<msg>.\r\n");

		std::string messageToTarget = ":" + origin.nickname + " PRIVMSG " + target->nickname + " :" + args[1];
		std::cout << origin << " send: \"" << messageToTarget << "\" to " << *target << std::endl;
		messageToTarget += "\r\n";
		target->sendMessage(messageToTarget);
	}
	return ("");
}
