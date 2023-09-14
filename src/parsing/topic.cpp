/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 18:20:31 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/14 18:47:58 by rsterin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

std::string cmdTopic(std::vector<std::string> &args, Client &origin, IrcServer &server)
{
	if (args.size() != 2)
		return ("Wrong number of arguments. Usage: TOPIC <channel> :<msg>.\r\n");

	Channel *channel;
	channel = server.getChannelWithName(args[0]);
	if (!channel || !channel->isUserInside(origin))
		return ("Channel not found. Please enter a valid channel using: TOPIC <channel> :<msg>.\r\n");

	if (channel->isMod(origin))
		channel->changeTopic(args[1]);
	else if (channel->isTopicChangeable)
		channel->changeTopic(args[1]);
	else
		return ("You cant change the topic.\r\n");

	return ("Channel's topic has been changed.\r\n");
}
