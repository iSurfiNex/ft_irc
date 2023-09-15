/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 17:20:30 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/14 19:48:56 by rsterin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

std::string cmdPart(strVec_t &args, Client &origin, IrcServer &server)
{
	if (args.size() < 1)
		return ("Wrong number of arguments. Usage: PART <channel>.\r\n");

	Channel *channel;
	channel = server.getChannelWithName(args[0]);
	if (!channel || !channel->isUserInside(origin))
		return ("Channel not found. Please enter a valid channel using: PART <channel>.\r\n");

	std::string messageToChannel = ":" + origin.nickname + " PART " + channel->name + " :has left the channel.\r\n";
	std::cout << origin << " left channel: " << channel->name << std::endl;
	channel->removeMod(origin);
	channel->removeUser(origin);
	channel->sendMessage(messageToChannel);
	return ("");
}
