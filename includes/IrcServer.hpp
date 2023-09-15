/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 17:32:13 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/14 19:42:42 by rsterin          ###   ########.fr       */
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
# include <vector>
# include <map>
# include <algorithm> // For std::find

# include "typedef.hpp"
# include "Channel.hpp"
# include "Client.hpp"
# include "utils.hpp"
# include "macros.hpp"

# define MAX_CLIENTS 30

class IrcServer
{
	public:
		typedef std::map<msgCode_e, std::string> msgMap_t;
		IrcServer(const int port, std::string &password);
		~IrcServer(void);
		void runServer(void);


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
		bool checkPassword(const std::string &pw);
		Client *getClientFromSocket(int sd);

		static msgMap_t msgFormats;
		static std::string formatCode(msgCode_e code, std::map<std::string, std::string> presets, va_list args);
		static std::string formatMsg(const std::string &format, std::map<std::string, std::string> presets, va_list args);
	private:
		int	_port;
		std::string &_password;
		void _initializeMsgFormats(void);
};

std::ostream	&operator <<(std::ostream &o, const IrcServer &irc);

int parsePort(char *av);
std::string parsePassword(char *av);
void getCmdArgs(std::string buffer, std::string &cmd, strVec_t &args);

std::string cmdPass(strVec_t &args, Client &origin, IrcServer &server);
std::string cmdUser(strVec_t &args, Client &origin, IrcServer &server);
std::string cmdNick(strVec_t &args, Client &origin, IrcServer &server);
std::string cmdPrivMsg(strVec_t &args, Client &origin, IrcServer &server);
std::string cmdPart(strVec_t &args, Client &origin, IrcServer &server);
std::string cmdInvite(strVec_t &args, Client &origin, IrcServer &server);
std::string cmdTopic(strVec_t &args, Client &origin, IrcServer &server);
std::string cmdKick(strVec_t &args, Client &origin, IrcServer &server);
std::string cmdMode(strVec_t &args, Client &origin, IrcServer &server);
void cmdJoin(strVec_t args, Client &client, IrcServer &server);

void parsing(Client &origin, IrcServer &server, std::string buffer);
