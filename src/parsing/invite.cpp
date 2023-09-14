/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 18:04:59 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/14 18:18:57 by rsterin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

std::string cmdInvite(std::vector<std::string> &args, Client &origin, IrcServer &server)
{
	if (args.size() != 1)
		return ("Wrong number of arguments. Usage: INVITE <nickname> <channel>.\r\n");

	Client *target;
	target = server.getClientWithNickname(args[0]);
	if (!target)
		return ("User not found . Please enter a valid target using: INVITE <nickname> <channel>.\r\n");

	Channel *channel;
	channel = server.getChannelWithName(args[1]);
	if (!channel || !channel->isUserInside(origin))
		return ("Channel not found. Please enter a valid channel using: INVITE <nickname> <channel>.\r\n");

	if (channel->isUserInside(*target))
		return ("User is already in targeted channel.\r\n");

	if (channel->isUserOnInviteList(*target))
		return ("User is already on the invite-list.\r\n");

	channel->addUserInviteList(*target);
}
