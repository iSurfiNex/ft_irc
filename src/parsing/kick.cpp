/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 18:38:10 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/14 18:48:27 by rsterin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

std::string cmdKick(std::vector<std::string> &args, Client &origin, IrcServer &server)
{
	if (args.size() != 2 || args.size() != 3)
		return ("Wrong number of arguments. Usage: KICK <channel> <nickname> (<reason>).\r\n");

	Channel *channel;
	channel = server.getChannelWithName(args[1]);
	if (!channel || !channel->isUserInside(origin))
		return ("Channel not found. Please enter a valid channel using: INVITE <nickname> <channel>.\r\n");

	if (channel->isMod(origin))
		return ("You are not a mod of the channel.\r\n");

	Client *target;
	target = server.getClientWithNickname(args[1]);
	if (!target)
		return ("User not found . Please enter a valid target using: INVITE <nickname> <channel>.\r\n");

	if (target->nickname == origin.nickname)
		return ("You can't kick yourself. Please enter a valid target using: INVITE <nickname> <channel>.\r\n");

	if (!channel->isUserInside(*target))
		return ("User is not in targeted channel.\r\n");

	if (channel->isMod(*target))
		return ("User is a mod of the targeted channel.\r\n");

	channel->removeUser(*target);
	std::string messageToOrigin = "You have successfully kicked " + target->nickname + " from the channel: " + channel->name + ".\r\n";
	std::cout << origin << " has kicked " << target << " from " << channel->name << std::endl;
	return (messageToOrigin);
}
