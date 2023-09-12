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

IrcServer::IrcServer(const int port, const std::string password)
{
	_port = port;
	_password = password;

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
	for (std::set<Client &>::iterator it = clients.begin(); clients.end() != it; ++it)
		delete &it;
	clients.clear();

	for (std::set<Channel &>::iterator it = channels.begin(); channels.end() != it; ++it)
		delete &it;
	channels.clear();
}

void IrcServer::runServer(void)
{
	int max_sd, sd, activity, valread, addrlen, new_socket;
    const char *welcome_message = "001 You are now connected. Please enter the password using: PASS <password>.\r\n";

    char buffer[1025];

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
			Client *tmp = new Client(new_socket);
			clients.insert(*tmp);

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

			if (FD_ISSET( sd , &readfds))
			{
				//Check if it was for closing , and also read the
				//incoming message
				if ((valread = read (sd , buffer, 1024)) == 0)
				{
					//Somebody disconnected , get his details and print
					getpeername(sd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
					std::cout << "Client disconnected, ip: " << inet_ntoa(address.sin_addr) << ", port: " << ntohs(address.sin_port) << "." << std::endl;

					//Close the socket and mark as 0 in list for reuse
					close (sd);
					client_socket[i] = 0;
				}

				//Echo back the message that came in
				else
				{
					//set the string terminating NULL byte on the end
					//of the data read
					buffer[valread] = '\0';
					std::cout << buffer;
					send(sd, buffer, strlen(buffer), 0);
				}
			}
		}
	}
}


std::ostream	&operator <<(std::ostream &o, const IrcServer &irc)
{
	(void)irc;
	return (o);
}
