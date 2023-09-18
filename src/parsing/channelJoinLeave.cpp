/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channelJoinLeave.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 18:41:01 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/18 18:17:36 by rsterin          ###   ########.fr       */
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
			origin.msg(ERR_NOSUCHNICK, &args[0]);
		else
		{
			Channel *channel;
			channel = server.getChannelWithName(args[1]);
			if (!channel || !channel->isUserInside(origin))
				origin.msg(ERR_NOSUCHCHANNEL, &args[1]);
			else if (!channel->isMod(origin))
				origin.msg(ERR_CHANOPRIVSNEEDED, &channel->name);
			else if (channel->isUserInside(*target))
				origin.msg(ERR_USERONCHANNEL, &target->nickname, &channel->name);
			else
			{
				channel->addUserInviteList(*target);
				origin.msg(RPL_INVITING, &target->nickname, &channel->name);
				target->msg(MSG_ADDINVITELIST, &origin.nickname, &channel->name);
			}
		}
	}
}

void cmdPart(strVec_t &args, Client &origin, IrcServer &server)
{
	if (args.size() < 1)
		origin.msg(ERR_NEEDMOREPARAMS, "PART");
	else
	{
		Channel *channel;
		channel = server.getChannelWithName(args[0]);
		if (!channel || !channel->isUserInside(origin))
			origin.msg(ERR_NOSUCHCHANNEL, &args[0]);
		else
		{
			origin.msg(MSG_PART, channel->name);
			channel->removeMod(origin);
			channel->removeUser(origin);
		}
	}
}

void cmdJoin(strVec_t args, Client &client, IrcServer &server)
{
	int argc = args.size();
	if (argc == 0)
		client.msg(ERR_NEEDMOREPARAMS, "JOIN");

	strVec_t chans;
	strVec_t keys;
	foreach(strVec_t, args)
	{
		std::string entry = *it;
		if (Channel::isValidName(entry))
		{
			std::string entryLower = tolowerStr(entry);
			if (std::find(chans.begin(), chans.end(), entryLower) != chans.end())
			{
				client.msg(ERR_BADCHANNELMASK, &entry, "Bad Channel Mask, same channel name provided multiple times");
				return;
			}
			chans.push_back(entryLower);
		}
		else if (Channel::isValidKey(entry))
			keys.push_back(entry);
		else
			client.msg(ERR_BADCHANNELMASK, &entry, "Bad Channel Mask");
	}
	if (chans.size() < keys.size())
	{
		std::string erroredEntry = keys[chans.size()];
		client.msg(ERR_BADCHANNELMASK, &erroredEntry, "Bad Channel Mask");
	}
	size_t i = 0;
	foreach(strVec_t, chans)
	{
		std::string chanName = *it;
		Channel *channel = server.getChannelWithName(chanName);
		std::string key = "";
		if (i < keys.size())
			key = keys[i];
		if (!channel)
			server.createChannel(client, chanName, key);
		else
			channel->tryEnter(client, key);
		i++;
	}
}
