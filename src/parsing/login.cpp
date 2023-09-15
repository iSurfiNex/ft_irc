/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   login.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 14:35:26 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/14 14:58:14 by rsterin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

std::string cmdPass(strVec_t &args, Client &origin, IrcServer &server)
{
	if (origin.isAuth)
	{
		std::cerr << origin << ": PASS: already logged in" << std::endl;
		return ("You are already logged in.\r\n");
	}
	if (args.size() != 1)
	{
		std::cerr << origin << ": PASS: Wrong number of arguments" << std::endl;
		return ("Wrong number of arguments. Usage: PASS <password>.\r\n");
	}
	else if (!server.checkPassword(args[0]))
	{
		std::cerr << origin << ": PASS: Wrong password" << std::endl;
		return ("Wrong password. Please enter the correct one using: PASS <password>.\r\n");
	}

	origin.isAuth = true;
	std::cout << origin << ": PASS: Now logged in" << std::endl;
	return ("You are now login, please use: USER <username> and NICK <nickname> to authentificate.\r\n");
}

std::string cmdUser(strVec_t &args, Client &origin, IrcServer &server)
{
	// Only consider the first agrument, ignore any remaining, full cmd could be "USER <username> <mode> <unused> :<realname>"
	if (args.size() < 1)
	{
		std::cerr << origin << ": USER: Wrong number of arguments" << std::endl;
		return ("Wrong number of arguments. Usage: USER <username>.\r\n");
	}

	Client *tmp = server.getClientWithUsername(args[0]);
	if (tmp != NULL)
	{
		std::cerr << origin << ": USER: Username already in use" << std::endl;
		return ("Username already in use. Please enter a unique one using: USER <username>.\r\n");
	}
	else if (!str_alnum(args[0]))
	{
		std::cerr << origin << ": USER: Username must contain only alpha-numeric character" << std::endl;
		return ("Username must contain only alpha-numeric character. Please enter a right one using: USER <username>.\r\n");
	}

	origin.changeUserName(args[0]);
	return ("");
}

std::string cmdNick(strVec_t &args, Client &origin, IrcServer &server)
{
	if (args.size() != 1)
	{
		std::cerr << origin << ": NICK: Wrong number of arguments" << std::endl;
		return ("Wrong number of arguments. Usage: NICK <nickname>.\r\n");
	}

	Client *tmp = server.getClientWithNickname(args[0]);
	if (tmp != NULL)
	{
		std::cerr << origin << ": NICK: Nickname already in use" << std::endl;
		return ("Nickname already in use. Please enter a unique one using: NICK <nickname>.\r\n");
	}
	else if (!str_alnum(args[0]))
	{
		std::cerr << origin << ": NICK: Nickname must contain only alpha-numeric character" << std::endl;
		return ("Nickname must contain only alpha-numeric character. Please enter a right one using: NICK <nickname>.\r\n");
	}

	origin.changeNickName(args[0]);
	return ("");
}
