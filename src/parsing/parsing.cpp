/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 17:31:25 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/12 19:13:46 by rsterin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

int parsePort(char *av)
{
	std::stringstream tmp(av);
	int port;

	if (!(tmp >> port) || !tmp.eof())
		return (-1);
	return (port);
}

std::string parsePassword(char *av)
{
	std::string password = av;

	if (password.find_first_of(" \a\b\t\n\v\f\r") != std::string::npos)
		return ("");
	return (password);
}

void parsing(Client &origin, Client &clients, Channel &channels, std::string buffer)
{
	std::string cmd;
	std::set<std::string> args;

	getCmdArgs(buffer, cmd, args);
	if (!origin.isAuth || !origin.isReady)
	{
		if (!origin.isAuth && cmd != "PASS")
		{
			std::cerr << "You did not enter the password. Please use: PASS <password> to login.\r\n";
			return ;
		}
		else if (origin.isAuth && (cmd != "USER" || cmd != "NICK"))
		{
			std::cerr << "You must set a username and a nickname. Please use: USER <username> and NICK <nickname> to authentificate.\r\n";
			return ;
		}
	}
	if (cmd == "PASS")
		cmdPass(cmd, args);
	else if (cmd == "USER")
		cmdUser(cmd, args);
	else if (cmd == "NICK")
		cmdNick(cmd, args);
	else if (cmd == "JOIN")
		cmdJoin(cmd, args);
	else if (cmd == "PART")
		cmdPart(cmd, args);
	else if (cmd == "PIVMSG")
		cmdPrivMsg(cmd, args);
	else if (cmd == "KICK")
		cmdKick(cmd, args);
	else if (cmd == "INVITE")
		cmdInvite(cmd, args);
	else if (cmd == "TOPIC")
		cmdTopic(cmd, args);
	else if (cmd == "MODE")
		cmdMode(cmd, args);
}
