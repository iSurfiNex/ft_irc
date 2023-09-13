/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 14:35:26 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/13 14:56:57 by rsterin          ###   ########.fr       */
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
