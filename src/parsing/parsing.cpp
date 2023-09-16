/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 17:31:25 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/16 15:47:13 by rsterin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IrcServer.hpp"
#include "IrcServer.hpp"

int parsePort(char *av)
{
	std::stringstream tmp(av);
	int port;

	if (av[0] == '\0' || !(tmp >> port) || !tmp.eof())
		return (-1);

	if (port < 1024 || port > 49151)
		return (-1);
	return (port);
}

std::string parsePassword(char *av)
{
	std::string password = av;

	if (password.find_first_of(" \a\b\t\n\v\f\r,") != std::string::npos || av[0] == '\0' || av[0] == '#')
		return ("");
	return (password);
}

static void _printCmd(Client &origin, std::string fullStr, std::string cmd, strVec_t args)
{

	std::cout << " >> " << cmd << "[";
	for (strVec_t::iterator it = args.begin(); args.end() != it;)
	{
		std::cout << *it ;
		++it;
		if(args.end() != it)
			std::cout << ", ";
	}
	std::cout << "] " GRAY "<= \"" << fullStr << "\"" NC " " << origin << std::endl;
}

void parsing(Client &origin, IrcServer &server, std::string buffer)
{
	std::string cmd;
	strVec_t args;

	getCmdArgs(buffer, cmd, args);

	_printCmd(origin, buffer, cmd, args);
	if (cmd == "CAP")
		return;

	if (!origin.isAuth || !origin.isReady)
	{
		if (!origin.isAuth && cmd != "PASS")
		{
			std::cerr << "User did not enter the password and is trying to send cmd.\r\n" << std::endl;
			origin.sendMessage("You did not enter the password. Please use: PASS <password> to login.\r\n");
			return ;
		}
		else if (origin.isAuth && cmd != "USER" && cmd != "NICK")
		{
			std::cerr << "User is not authentificate and is trying to send cmd.\r\n" <<std::endl;
			origin.sendMessage("You must set a username and a nickname. Please use: USER <username> and NICK <nickname> to authentificate..\r\n");
			return ;
		}
	}

	if (cmd == "PASS")
		cmdPass(args, origin, server);
	else if (cmd == "USER")
		cmdUser(args, origin, server);
	else if (cmd == "NICK")
		cmdNick(args, origin, server);
	else if (cmd == "PRIVMSG")
		cmdPrivMsg(args, origin, server);
	else if (cmd == "JOIN")
		cmdJoin(args, origin, server);
	else if (cmd == "PART")
		cmdPart(args, origin, server);
	else if (cmd == "INVITE")
		cmdInvite(args, origin, server);
	else if (cmd == "TOPIC")
		cmdTopic(args, origin, server);
	else if (cmd == "KICK")
		cmdKick(args, origin, server);
	else if (cmd == "MODE")
		cmdMode(args, origin, server);
}
