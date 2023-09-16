/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 17:20:30 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/16 18:39:52 by rsterin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

void cmdPart(strVec_t &args, Client &origin, IrcServer &server)
{
	if (args.size() < 1)
		origin.msg(ERR_NEEDMOREPARAMS, "PART");
	else
	{
		Channel *channel;
		channel = server.getChannelWithName(args[0]);
		if (!channel || !channel->isUserInside(origin))
			origin.msg(ERR_NOSUCHCHANNEL, args[0]);
		else
		{
			channel->removeMod(origin);
			channel->removeUser(origin);
		}
	}
}
