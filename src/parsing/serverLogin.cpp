/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   login.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 14:35:26 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/17 18:12:47 by rsterin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

void cmdPass(strVec_t &args, Client &origin, IrcServer &server)
{
	if (origin.isAuth)
		origin.msg(ERR_ALREADYREGISTERED);
	else if (args.size() < 1)
		origin.msg(ERR_NEEDMOREPARAMS, "PART");
	else if (!server.checkPassword(args[0]))
		origin.msg(ERR_PASSWDMISMATCH);
	else
	{
		origin.isAuth = true;
		std::cout << GREEN "New user logged in: " NC << origin << std::endl;
		origin.sendMessage("You are now login, please use: USER <username> and NICK <nickname> to authentificate.\r\n");
	}
}

void cmdUser(strVec_t &args, Client &origin, IrcServer &server)
{
	if (origin.isReady)
		origin.msg(ERR_ALREADYREGISTERED);
	else if (args.size() < 1)
		origin.msg(ERR_NEEDMOREPARAMS, "USER");
	else
	{
		if (args[0].find_first_of("\a: ") != std::string::npos)
		{
			std::cerr << origin << ": USER: Username must contain only alpha-numeric character" << std::endl;
			origin.sendMessage(origin.nickname + ": Username must contain only alpha-numeric character.\r\n");
		}
		else
			origin.changeUserName(args[0]);
	}
	(void)server;
}

void cmdNick(strVec_t &args, Client &origin, IrcServer &server)
{
	if (args.size() < 1)
		origin.msg(ERR_NONICKNAMEGIVEN);
	else if (args[0].find_first_of("\a: ") != std::string::npos)
		origin.msg(ERR_ERRONEUSNICKNAME, args[0]);
	else
	{
		Client *tmp = server.getClientWithNickname(args[0]);
		if (tmp != NULL)
			origin.msg(ERR_NICKNAMEINUSE, args[0]);
		else
			origin.changeNickName(args[0]);
	}
}
