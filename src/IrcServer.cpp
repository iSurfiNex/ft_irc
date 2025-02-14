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
	m[ERR_NOSUCHCHANNEL] = "<server> <code> <client> <channel> :No such channel";
	m[ERR_TOOMANYCHANNELS] = "<server> <code> <client> <channel> :You have joined too many channels";
	m[ERR_BADCHANNELKEY] = "<server> <code> <client> <channel> :Cannot join channel (+k)";
	m[ERR_BADCHANNELMASK] = "<server> <code> <channel> :<reason_cstr>";
	m[ERR_CHANNELISFULL] = "<server> <code> <client> <channel> :Cannot join channel (+l)";
	m[ERR_INVITEONLYCHAN] = "<server> <code> <client> <channel> :Cannot join channel (+i)";
	m[ERR_USERLIMIT] = "<server> <code> <client> <channel> :Cannot join channel (+l)";
	m[ERR_NEEDMOREPARAMS] = "<server> <code> <client> <command_cstr> :Not enough parameters";
	m[RPL_TOPIC] = "<server> <code> <client> <channel> :<topic>";
	m[RPL_NOTOPIC] = "<server> <code> <client> <channel> :No topic is set";
	m[RPL_ENDOFNAMES] = "<server> <code> <client> <channel> :End of /NAMES list";
	m[RPL_NAMREPLY] = "<server> <code> <client> <symbol> <channel> :<nick>";
	m[ERR_PASSWDMISMATCH] = "<server> <code> <client> :Password incorrect";
	m[ERR_ALREADYREGISTERED] = "<server> <code> <client> :You may not reregister";
	m[ERR_NONICKNAMEGIVEN] = "<server> <code> <client> :No nickname given";
	m[ERR_ERRONEUSNICKNAME] = "<server> <code> <client> <nick> :Erroneus nickname";
	m[ERR_NICKNAMEINUSE] = "<server> <code> <client> <nick> :Nickname already in use";
	m[ERR_NOTEXTTOSEND] = "<server> <code> <client> :No text to send";
	m[ERR_CANNOTSENDTOCHAN] = "<server> <code> <client> <channel> :Cannot send to channel";
	m[ERR_NOSUCHNICK] = "<server> <code> <client> <nickname> :No such nick/channel";
	m[ERR_CHANOPRIVSNEEDED] = "<server> <code> <client> <channel> :You're not channel operator";
	m[ERR_USERNOTINCHANNEL] = "<server> <code> <client> <nick> <channel> :They aren't on that channel";
	m[ERR_NOTONCHANNEL] = "<server> <code> <client> <channel> :They aren't on that channel";
	m[ERR_USERONCHANNEL] = "<server> <code> <client> <nick> <channel> :is already on channel";
	m[RPL_INVITING] = "<server> <code> <client> <nick> <channel>";
	m[RPL_WELCOME] = "<server> <code> <client> :Welcome to the <networkname> Network, <client>";
	m[MSG_JOIN] = "<client> JOIN :<channel>";
	m[MSG_PART] = "<client> PART :<channel>";
	m[MSG_MODE] = "<client> MODE :<channel> <modeargs>";
	m[MSG_ADDINVITELIST] = "<server> <code> <client> <nick> <channel> :invite you";
	m[ERR_NOTVALIDENTRY] = "<server> <code> <channel> :\"<entry>\" is not a valid entry";
	msgFormats = m;
}


IrcServer::IrcServer(const int port, const std::string &_name, const std::string &_networkName, std::string &password, int maxClient, int _defaultMaxClientPerChan, int _defaultMaxChanPerClient): name(_name), networkName(_networkName), defaultMaxClientPerChan(_defaultMaxClientPerChan), defaultMaxChanPerClient(_defaultMaxChanPerClient), _port(port), _password(password), _maxClient(maxClient)
{
	_initializeMsgFormats();
	_initializeServer();
}

IrcServer::~IrcServer(void)
{
	foreach(clientSet_t, clients)
		delete *it;
	clients.clear();

	foreach(clientSet_t, clients)
		delete *it;
	channels.clear();
}

void IrcServer::_initializeServer()
{
	if (!(master_socket = socket(AF_INET , SOCK_STREAM , 0)))
		throw (std::runtime_error("IRC: socket failure"));

	int opt = 1;
	if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
		throw (std::runtime_error("IRC: socket failure"));

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(_port);

	if (bind(master_socket, reinterpret_cast<struct sockaddr *>(&address), sizeof(address)) < 0)
		throw (std::runtime_error("IRC: bind failure"));

	if (listen(master_socket, 3) < 0)
		throw (std::runtime_error("IRC: listen failure"));

	std::cout << std::endl << "  Waiting for connections on port " << _port << " using \"" << _password << "\" as password..." << std::endl << std::endl;
}

/* Handle incomming connection and create associated client */
void IrcServer::_handleIncomingConnection()
{
	int newSd;
    socklen_t addrlen = sizeof(address);

	// Return if no incomming connection
	if (!FD_ISSET(master_socket, &readfds))
		return;

	newSd = accept(master_socket, reinterpret_cast<struct sockaddr *>(&address), &addrlen);

	if (newSd < 0)
		throw (std::runtime_error("IRC: accept failure"));

	if (clients.size() >= static_cast<size_t>(_maxClient))
	{
		std::cout << RED "Rejected connection, max client reach (" << _maxClient << ") " << NC << _connectionToString(newSd, address);
		return;
	}

	std::cout << " " FGREEN "New connection:" NC " " << _connectionToString(newSd, address);

	Client *newClient = new Client(newSd, address, *this);
	clients.insert(newClient);
}


void IrcServer::_handleIOOperation()
{
	char buffer[BUFFER_SIZE];

	clientSet_t::iterator it = clients.begin();
	while (it != clients.end())
	{
		Client *client = *it;
		int sd = client->socketId;

		if (!FD_ISSET(sd, &readfds))
		{
			++it;
			continue ;
		}

		int socketContentSize = read(sd, buffer, BUFFER_SIZE);

		if (socketContentSize < 1)
		{
			std::cout << " " FRED "Client disconnected:" NC " " << _connectionToString(sd, address);
			it++; // Increment before erasing from the set we are currently iterating
			clients.erase(client);
			delete client;
			close (sd);
			continue ;
		}

		int skipCount = 2;
		buffer[socketContentSize] = '\0';
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
					std::cout << " >> Partial message \"" GRAY << client->partialMsg << NC "\" from " << *client << std::endl;
				break;
			}
			std::string cmd_str = str_msg.substr(start_pos, end_pos - start_pos);

			parsing(*client, *this, cmd_str);

			start_pos = end_pos + skipCount;
		}
		++it;
	}
}

void IrcServer::runServer(void)
{
	//highest file descriptor number, need it for the select function
	int max_sd;
	int activity;

	while(true)
	{
		//clear the socket set
		FD_ZERO(&readfds);

		//add master socket to set
		FD_SET(master_socket, &readfds);
		max_sd = master_socket;

		foreach(clientSet_t, clients)
		{
			int sd = (*it)->socketId;
			FD_SET(sd, &readfds);
			if(sd > max_sd)
				max_sd = sd;
		}

		//wait for an activity on one of the sockets , timeout is NULL ,
		//so wait indefinitely
		activity = select(max_sd + 1 , &readfds , NULL , NULL , NULL);

		if ((activity < 0) && (errno!=EINTR))
			throw (std::runtime_error("IRC: select failure"));

		_handleIncomingConnection();
		_handleIOOperation();
	}
}

/* Get the channels list which client is part of. */
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

std::string IrcServer::_connectionToString(int sd, struct sockaddr_in &address)
{
	std::stringstream ss;
	ss << "socket FD: " GRAY << sd << NC " ─ IP: " GRAY << inet_ntoa(address.sin_addr) << NC " ─ PORT: " GRAY << ntohs(address.sin_port) << NC "." << std::endl;
	return ss.str();
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
	if (mod.getChanCount() >= mod.maxChans)
	{
		mod.msg(ERR_TOOMANYCHANNELS, &name);
		return NULL;
	}
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


std::string IrcServer::formatCode(int code, std::map<std::string, std::string> presets, va_list args)
{
	const std::string& format = IrcServer::msgFormats[(msgCode_e)code];
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
		{
			repl_str = *va_arg(args, std::string *);
		}

		eval.replace(repl_start, repl_len, repl_str);
		repl_start = repl_end;
	}

	return eval;
}
