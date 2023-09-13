/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privMsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 16:58:52 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/13 18:11:04 by rsterin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

std::string cmdPrivMsg(std::vector<std::string> &args, Client &origin, IrcServer &server)
{
	if (args.size() != 2)
		return ("Wrong number of arguments. Usage: PRIVMSG <channel/nickname> : <msg>.\r\n");

	if (args[0][0] == '#')
	{
		Channel *channel;
		channel = server.getChannelWithName(args[0]);
		if (!channel || !channel->isUserInside(origin))
			return ("Channel not found. Please enter a valid channel using: PRIVMSG <channel/nickname> : <msg>.\r\n");

	}
	else
	{

	}
}
