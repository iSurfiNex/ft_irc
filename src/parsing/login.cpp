/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   login.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 14:35:26 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/13 15:48:30 by rsterin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

std::string cmdPass(std::string &cmd, std::set<std::string> &args, Client &origin, IrcServer &server)
{
	if (args.size() != 1)
		return ("Wrong number of arguments. Usage: PASS <password>.\r\n");
	else if (args.find(server._password) == args.end())
		return ("Wrong password. Please enter the correct one using: PASS <password>.\r\n");

	origin.isAuth = true;
	return ("You are now login, please use: USER <username> and NICK <nickname> to authentificate.");
}

std::string cmdUser(std::string &cmd, std::set<std::string> &args, Client &origin, IrcServer &server)
{
	if (args.size() != 1)
		return ("Wrong number of arguments. Usage: USER <username>.\r\n");

	Client *tmp = server.getClientWithUsername(*args.begin());
	if (tmp != NULL)
		return ("Username already in use. Please enter a unique one using: USER <username>.\r\n");

	origin.changeUserName(*args.begin());
	return ("");
}

std::string cmdNick(std::string &cmd, std::set<std::string> &args, Client &origin, IrcServer &server)
{
	if (args.size() != 1)
		return ("Wrong number of arguments. Usage: NICK <nickname>.\r\n");

	Client *tmp = server.getClientWithNickname(*args.begin());
	if (tmp != NULL)
		return ("Nickname already in use. Please enter a unique one using: NICK <nickname>.\r\n");

	origin.changeNickName(*args.begin());
	return ("");
}
