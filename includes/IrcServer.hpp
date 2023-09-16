/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 17:32:13 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/15 15:58:00 by rsterin          ###   ########.fr       */
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
# include "parsing.hpp"

# define MAX_CLIENTS 30

class IrcServer
{
	public:

		/* TYPING  */

		typedef std::map<msgCode_e, std::string> msgMap_t;

		/* LIFECYCLE */

		IrcServer(const int port, const std::string &_name, std::string &password);
		~IrcServer(void);

		/* ACTIONS */

		void runServer(void);
		Channel *createChannel(Client &mod, const std::string &channelName, const std::string& key);

		/* SELECTORS */

		Channel *getChannelWithName(const std::string &name);
		Client *getClientWithNickname(const std::string &name);
		Client *getClientWithUsername(const std::string &name);
		Client *getClientFromSocket(int sd);
		chanSet_t getUserChans(const Client &client) const;

		/* CHECKS */

		bool checkPassword(const std::string &pw);

		/* ATTRIBUTES */

		fd_set readfds;
		int master_socket;
		int client_socket[MAX_CLIENTS];
		struct sockaddr_in address;

		clientSet_t clients;
		chanSet_t channels;

		std::string name;

		/* STATIC ATTRIBUTES */

		static msgMap_t msgFormats;
		static std::string formatCode(msgCode_e code, std::map<std::string, std::string> presets, va_list args);
		static std::string formatMsg(const std::string &format, std::map<std::string, std::string> presets, va_list args);

	private:

		/* ATTRIBUTES */

		int	_port;
		std::string &_password;
		void _initializeMsgFormats(void);
};

std::ostream	&operator <<(std::ostream &o, const IrcServer &irc);

