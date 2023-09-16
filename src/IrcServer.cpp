/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   IrcServer.cpp									  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/09/11 18:06:13 by rsterin		   #+#	#+#			 */
/*   Updated: 2023/09/11 19:22:24 by rsterin		  ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "IrcServer.hpp"
IrcServer::msgMap_t IrcServer::msgFormats;

void IrcServer::_initializeMsgFormats(void) {

	msgMap_t m;
	m[ERR_NOSUCHCHANNEL] = "<server> <code> ERR_NOSUCHCHANNEL <client> <channel> :No such channel";
	m[ERR_TOOMANYCHANNELS] = "<server> <code> ERR_TOOMANYCHANNELS <client> <channel> :You have joined too many channels";
	m[ERR_BADCHANNELKEY] = "<server> <code> ERR_BADCHANNELKEY <client> <channel> :Cannot join channel (+k)";
	m[ERR_BADCHANNELMASK] = "<server> <code> ERR_BADCHANNELMASK <channel> :<reason_cstr>";
	m[ERR_CHANNELISFULL] = "<server> <code> ERR_CHANNELISFULL <client> <channel> :Cannot join channel (+l)";
	m[ERR_INVITEONLYCHAN] = "<server> <code> ERR_INVITEONLYCHAN <client> <channel> :Cannot join channel (+i)";
	m[ERR_NEEDMOREPARAMS] = "<server> <code> ERR_NEEDMOREPARAMS <client> <command_cstr> :Not enough parameters";
	m[RPL_TOPIC] = "<server> <code> RPL_TOPIC <client> <channel> :<topic>";
	m[RPL_NOTOPIC] = "<server> <code> RPL_NOTOPIC <client> <channel> :No topic is set";
	m[RPL_ENDOFNAMES] = "<server> <code> RPL_ENDOFNAMES <client> <channel> :End of /NAMES list";
	m[RPL_NAMREPLY] = "<server> <code> RPL_NAMREPLY <client> <symbol> <channel> :<nick>";
	m[ERR_PASSWDMISMATCH] = "<server> <code> ERR_PASSWDMISMATCH <client> :Password incorrect";
	m[ERR_ALREADYREGISTERED] = "<server> <code> ERR_ALREADYREGISTERED <client> :You may not reregister";
	m[ERR_NONICKNAMEGIVEN] = "<server> <code> ERR_NONICKNAMEGIVEN <client> :No nickname given";
	m[ERR_ERRONEUSNICKNAME] = "<server> <code> ERR_ERRONEUSNICKNAME <client> <nick> :Erroneus nickname";
	m[ERR_NICKNAMEINUSE] = "<server> <code> ERR_NICKNAMEINUSE <client> <nick> :Nickname already in use";
	m[ERR_NOTEXTTOSEND] = "<server> <code> ERR_NOTEXTTOSEND <client> :No text to send";
	m[ERR_CANNOTSENDTOCHAN] = "<server> <code> ERR_CANNOTSENDTOCHAN <client> <channel> :Cannot send to channel";
	m[ERR_NOSUCHNICK] = "<server> <code> ERR_NOSUCHNICK <client> <nickname> :No such nick/channel";
	m[ERR_CHANOPRIVSNEEDED] = "<server> <code> ERR_CHANOPRIVSNEEDED <client> <channel> :You're not channel operator";
	m[ERR_USERNOTINCHANNEL] = "<server> <code> ERR_USERNOTINCHANNEL <client> <nick> <channel> :They aren't on that channel";
	m[ERR_NOTONCHANNEL] = "<server> <code> ERR_NOTONCHANNEL <client> <channel> :They aren't on that channel";
	m[ERR_USERONCHANNEL] = "<server> <code> ERR_USERONCHANNEL <client> <nick> <channel> :is already on channel";
	m[RPL_INVITING] = "<server> <code> RPL_INVITING <client> <nick> <channel>";
	m[MSG_JOIN] = "<client> JOIN <channel>";
	m[MSG_ADDINVITELIST] = "<server> <code> MSG_ADDINVITELIST <client> <nick> <channel> :invite you";
	m[MSG_NEWTOPIC] = "<server> <code> MSG_NEWTOPIC <channel> :\"<topic>\" is the new topic of this channel";
	msgFormats = m;
}

IrcServer::IrcServer(const int port, const std::string &_name, std::string &password): name(_name), _password(password)
{
	_initializeMsgFormats();
	_port = port;

	std::cout << "PORT: " << port << " | PASSWORD: " << password << std::endl << std::endl;

	for (int i = 0; i < MAX_CLIENTS; i++)
		client_socket[i] = 0;

	if (!(master_socket = socket(AF_INET , SOCK_STREAM , 0)))
		throw (std::runtime_error("IRC: socket failure"));


	int opt = 1;
	if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
		throw (std::runtime_error("IRC: socket failure"));

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(_port);

	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
		throw (std::runtime_error("IRC: bind failure"));

	if (listen(master_socket, 3) < 0)
		throw (std::runtime_error("IRC: listen failure"));

	std::cout << "  Waiting for connections..." << std::endl << std::endl;
}

IrcServer::~IrcServer(void)
{
	for (std::set<Client *>::iterator it = clients.begin(); clients.end() != it; ++it)
		delete *it;
	clients.clear();

	for (chanSet_t::iterator it = channels.begin(); channels.end() != it; ++it)
		delete *it;
	channels.clear();
}


/* Returns the channels list which client is part of. */
chanSet_t IrcServer::getUserChans(const Client &client) const
{
	chanSet_t userChans;
	foreach(chanSet_t, channels)
	{
		Channel *chan = *it;
		if (chan->isUserInside(client))
			userChans.insert(chan);
	}
	return userChans;
}

void IrcServer::runServer(void)
{
	int max_sd, sd, activity, valread, addrlen, new_socket;
    const char *welcome_message = "You are now connected. Please enter the password using: PASS <password>.\r\n";

    char buffer[1025];
    addrlen = sizeof(address);

	while(true)
	{
		//clear the socket set
		FD_ZERO(&readfds);

		//add master socket to set
		FD_SET(master_socket, &readfds);
		max_sd = master_socket;

		//add child sockets to set
		for (int i = 0; i < MAX_CLIENTS; i++)
		{
			//socket descriptor
			sd = client_socket[i];

			//if valid socket descriptor then add to read list
			if (sd > 0)
				FD_SET(sd, &readfds);

			//highest file descriptor number, need it for the select function
			if(sd > max_sd)
				max_sd = sd;
		}

		//wait for an activity on one of the sockets , timeout is NULL ,
		//so wait indefinitely
		activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);

		if ((activity < 0) && (errno!=EINTR))
			throw (std::runtime_error("IRC: select failure"));

		//If something happened on the master socket ,
		//then its an incoming connection
		if (FD_ISSET(master_socket, &readfds))
		{
			if ((new_socket = accept(master_socket,	(struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
				throw (std::runtime_error("IRC: accept failure"));

			//inform user of socket number - used in send and receive commands
			std::cout << "New connection, socket fd: " << new_socket << ", ip: " << inet_ntoa(address.sin_addr) << ", port: " << ntohs(address.sin_port)  << "." << std::endl;
			Client *tmp = new Client(new_socket, *this);
			clients.insert(tmp);

			//send new connection greeting message
			if (send(new_socket, welcome_message, strlen(welcome_message), 0) != static_cast<ssize_t>(strlen(welcome_message)) )
				throw (std::runtime_error("IRC: send failure"));

			//add new socket to array of sockets
			for (int i = 0; i < MAX_CLIENTS; i++)
			{
				//if position is empty
				if (client_socket[i] == 0 )
				{
					client_socket[i] = new_socket;
					std::cout << "Adding to list of sockets as " << i << "." << std::endl;

					break ;
				}
			}
		}

		//else its some IO operation on some other socket
		for (int i = 0; i < MAX_CLIENTS; i++)
		{
			sd = client_socket[i];
			if (!sd)
				continue;

			Client *client= getClientFromSocket(sd);
			if (!client)
			{
				std::cerr << "No client for socket " << sd << std::endl;
				break;
			}
			if (FD_ISSET(sd, &readfds))
			{
				//Check if it was for closing , and also read the
				//incoming message
				if ((valread = read(sd, buffer, 1024)) == 0)
				{
					//Somebody disconnected , get his details and print
					getpeername(sd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
					std::cout << "Client disconnected, ip: " << inet_ntoa(address.sin_addr) << ", port: " << ntohs(address.sin_port) << "." << std::endl;
					clients.erase(client);
					delete client;

					//Close the socket and mark as 0 in list for reuse
					close (sd);
					client_socket[i] = 0;
				}

				//Echo back the message that came in
				else
				{
					int skipCount = 2;
					buffer[valread] = '\0';
					std::string str_msg = client->partialMsg + buffer;
					long unsigned int start_pos = 0;
					long unsigned int end_pos = 0;
					while (true)
					{
						end_pos = str_msg.find("\r\n", start_pos);
						if (end_pos == std::string::npos)
						{
							end_pos = str_msg.find("\n", start_pos);
							skipCount = 1;
						}

						if (end_pos == std::string::npos)
						{
							client->partialMsg = str_msg.substr(start_pos, std::string::npos);
							if (!client->partialMsg.empty())
								std::cout <<  GRAY " >> Partial message \"" NC << client->partialMsg << GRAY "\" from " NC << *client << std::endl;
							break;
						}
						std::string cmd_str = str_msg.substr(start_pos, end_pos - start_pos);

						parsing(*client, *this, cmd_str);

						start_pos = end_pos + skipCount;
					}
				}
			}
		}
	}
}

Channel *IrcServer::getChannelWithName(const std::string &channelName)
{
	for (chanSet_t::iterator it = channels.begin(); channels.end() != it; ++it)
	{
		Channel *channel = *it;
		if (channel->name == channelName)
			return channel;
	}
	return NULL;
}


Client *IrcServer::getClientWithUsername(const std::string &name)
{
	for (std::set<Client *>::iterator it = clients.begin(); clients.end() != it; ++it)
	{
		Client *client = *it;
		if (client->username == name)
			return client;
	}
	return NULL;
}

Client *IrcServer::getClientWithNickname(const std::string &name)
{
	for (std::set<Client *>::iterator it = clients.begin(); clients.end() != it; ++it)
	{
		Client *client = *it;
		if (client->nickname == name)
			return client;
	}
	return NULL;
}

Channel *IrcServer::createChannel(Client &mod, const std::string &channelName, const std::string& key)
{
	Channel *channel = new Channel(channelName, key, mod, *this);
	channels.insert(channel);
	return channel;
}

bool IrcServer::checkPassword(const std::string &pw)
{
	return pw == _password;
}

std::ostream	&operator <<(std::ostream &o, const IrcServer &irc)
{
	(void)irc;
	return (o);
}

Client *IrcServer::getClientFromSocket(int sd)
{
	for (std::set<Client *>::iterator it = clients.begin(); clients.end() != it; ++it)
	{
		Client *client = *it;
		if (client->socketId == sd)
			return client;
	}
	return NULL;
}


std::string IrcServer::formatCode(msgCode_e code, std::map<std::string, std::string> presets, va_list args)
{
	const std::string& format = IrcServer::msgFormats[code];
	return IrcServer::formatMsg(format, presets, args);
}

std::string IrcServer::formatMsg(const std::string &format, std::map<std::string, std::string> presets, va_list args)
{
	std::string eval = format;

	while(true)
	{
		size_t repl_start = eval.find("<");
		if (repl_start == std::string::npos)
			break;
		size_t repl_end = eval.find(">", repl_start);
		if (repl_end == std::string::npos)
			break;
		size_t repl_len = repl_end - repl_start + 1;

		std::string key = eval.substr(repl_start, repl_len);
		std::string repl_str;
		if (presets.find(key) != presets.end())
			repl_str = presets[key];
		else if (key.find("cstr") != std::string::npos)
			repl_str = std::string(va_arg(args, const char *));
		else
			repl_str = va_arg(args, std::string);

		eval.replace(repl_start, repl_len, repl_str);
		repl_start = repl_end;
	}

	return eval;
}
