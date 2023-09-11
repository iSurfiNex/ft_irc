/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 17:25:52 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/11 19:42:39 by rsterin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << "Error:\n  Usage " << av[0] << " <port> <password>\n  Port should be between X and Y.\n" << std::endl;
		return (1);
	}

	// TODO better checker for error (argc, correct password...)
	int port = parsePort(av[1]);
	std::string password = parsePassword(av[2]);
	if (port == -1 || password.empty())
	{
		std::cerr << "Error on parsing" << std::endl;
		return (1);
	}

	try
	{
		IrcServer irc(port, password);
		irc.runServer();
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}

	return (0);
}
