/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 17:31:25 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/13 16:33:56 by rsterin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IrcServer.hpp"
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

	if (password.find_first_of(" \a\b\t\n\v\f\r") != std::string::npos || password[0] == NULL)
		return ("");
	return (password);
}

void parsing(Client &origin, IrcServer &server, std::string buffer)
{
	std::string cmd;
	std::vector<std::string> args;

	std::string response;

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
		response = cmdPass(args, origin, server);
	else if (cmd == "USER")
		response = cmdUser(args, origin, server);
	else if (cmd == "NICK")
		response = cmdNick(args, origin, server);
	// else if (cmd == "JOIN")
	// 	response = cmdJoin(args, origin, server);
	// else if (cmd == "PART")
	// 	response = cmdPart(args, origin, server);
	// else if (cmd == "PIVMSG")
	// 	response = cmdPrivMsg(args, origin, server);
	// else if (cmd == "KICK")
	// 	response = cmdKick(args, origin, server);
	// else if (cmd == "INVITE")
	// 	response = cmdInvite(args, origin, server);
	// else if (cmd == "TOPIC")
	// 	response = cmdTopic(args, origin, server);
	// else if (cmd == "MODE")
	// 	response = cmdMode(args, origin, server);

	// client.send(response);
}
