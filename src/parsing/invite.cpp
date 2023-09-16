/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 18:04:59 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/16 19:07:21 by rsterin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

void cmdInvite(strVec_t &args, Client &origin, IrcServer &server)
{
	if (args.size() < 2)
		origin.msg(ERR_NEEDMOREPARAMS, "INVITE");
	else
	{
		Client *target;
		target = server.getClientWithNickname(args[0]);
		if (!target)
			origin.msg(ERR_NOSUCHNICK, args[0]);
		else
		{
			Channel *channel;
			channel = server.getChannelWithName(args[1]);
			if (!channel || !channel->isUserInside(origin))
				origin.msg(ERR_NOSUCHCHANNEL, args[1]);
			else if (!channel->isMod(origin))
				origin.msg(ERR_CHANOPRIVSNEEDED, channel->name);
			else if (channel->isUserInside(*target))
				origin.msg(ERR_USERONCHANNEL, target->nickname, channel->name);
			else
			{
				channel->addUserInviteList(*target);
				origin.msg(RPL_INVITING, target->nickname, channel->name);
				target->msg(MSG_ADDINVITELIST, origin.nickname, channel->name);
			}
		}
	}
}
