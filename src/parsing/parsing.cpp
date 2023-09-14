/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 17:31:25 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/14 18:47:14 by rsterin          ###   ########.fr       */
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

	if (password.find_first_of(" \a\b\t\n\v\f\r") != std::string::npos || av[0] == '\0')
		return ("");
	return (password);
}

static void _printCmd(std::string fullStr, std::string cmd, std::vector<std::string> args)
{

	std::cout <<" >> " << cmd << "[";
	for (std::vector<std::string>::iterator it = args.begin(); args.end() != it;)
	{
		std::cout << *it ;
		++it;
		if(args.end() != it)
			std::cout << ", ";
	}
	std::cout << "] " GRAY "<= \"" << fullStr << "\"" NC << std::endl;
}

void parsing(Client &origin, IrcServer &server, std::string buffer)
{
	std::string cmd;
	std::vector<std::string> args;

	std::string response;

	getCmdArgs(buffer, cmd, args);

	_printCmd(buffer, cmd, args);
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
		response = cmdPass(args, origin, server);
	else if (cmd == "USER")
		response = cmdUser(args, origin, server);
	else if (cmd == "NICK")
		response = cmdNick(args, origin, server);
	else if (cmd == "PRIVMSG")
		response = cmdPrivMsg(args, origin, server);
	else if (cmd == "JOIN")
		response = cmdJoin(args, origin, server);
	else if (cmd == "PART")
		response = cmdPart(args, origin, server);
	else if (cmd == "INVITE")
		response = cmdInvite(args, origin, server);
	else if (cmd == "TOPIC")
		response = cmdTopic(args, origin, server);
	else if (cmd == "KICK")
		response = cmdKick(args, origin, server);
	// else if (cmd == "MODE")
	// 	response = cmdMode(args, origin, server);

	origin.sendMessage(response);
}
