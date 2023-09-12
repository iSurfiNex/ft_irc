/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 17:32:13 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/12 15:43:20 by rsterin          ###   ########.fr       */
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

# include "Channel.hpp"
# include "Client.hpp"

# define MAX_CLIENTS 30

class IrcServer
{
	public:
		IrcServer(const int port, const std::string password);
		~IrcServer(void);
		void runServer(void);


		fd_set readfds;
		int master_socket;
		int client_socket[MAX_CLIENTS];
		struct sockaddr_in address;
		std::set<Client &> clients;
		std::set<Channel &> channels;

	private:
		int	_port;
		std::string _password;
};

std::ostream	&operator <<(std::ostream &o, const IrcServer &irc);

int parsePort(char *av);
std::string parsePassword(char *av);
