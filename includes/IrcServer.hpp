/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 17:32:13 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/13 15:57:22 by rsterin          ###   ########.fr       */
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

		static std::string _password;

		fd_set readfds;
		int master_socket;
		int client_socket[MAX_CLIENTS];
		struct sockaddr_in address;

		std::set<Client *> clients;
		std::set<Channel *> channels;

		Channel *getChannelWithName(const std::string &name);
		Client *getClientWithUsername(const std::string &name);
		Client *getClientWithNickname(const std::string &name);
		Channel *createChannel(const std::string &channelName, Client &mod);

	private:
		int	_port;
};

std::ostream	&operator <<(std::ostream &o, const IrcServer &irc);

int parsePort(char *av);
std::string parsePassword(char *av);
void getCmdArgs(std::string buffer, std::string &cmd, std::set<std::string> &args);

std::string cmdPass(std::set<std::string> &args, Client &origin, IrcServer &server);
std::string cmdUser(std::set<std::string> &args, Client &origin, IrcServer &server);
std::string cmdNick(std::set<std::string> &args, Client &origin, IrcServer &server);
