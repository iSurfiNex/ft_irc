/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 17:32:13 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/11 19:52:20 by rsterin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include <sstream>
# include <string.h>
# include <sys/time.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <errno.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <set>

# define MAX_CLIENTS 30

class IrcServer
{
	public:
		// IrcServer(void);
		IrcServer(const int port, const std::string password);
		~IrcServer(void);
		void runServer(void);


		fd_set readfds;
		int master_socket;
		int client_socket[MAX_CLIENTS];
		struct sockaddr_in address;

	private:
		int	_port;
		std::string _password;
};

std::ostream	&operator <<(std::ostream &o, const IrcServer &irc);

int parsePort(char *av);
std::string parsePassword(char *av);
