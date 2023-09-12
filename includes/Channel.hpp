/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 19:55:19 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/12 15:13:18 by rsterin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "IrcServer.hpp"
# include "Client.hpp"

class Channel
{
	public:
		Channel(const std::string cName);
		~Channel(void);

		void addUser(const Client &client);
		void addMod(const Client &client);

		void removeUser(const Client &client);
		void removeMod(const Client &client);

		void changeRule(const char mode);
		void changeTopic(const std::string topic);
		void changePassword(const std::string password);
		void changeUserLimit(const int userlimit);

		bool isMod(const Client &client);
		bool isUserInside(const Client &client);

		std::string channelname;

	private:
		std::set<const Client &> _userList;
		std::set<const Client &> _modList;

		bool _isInviteOnly;
		bool _isTopicChangeable;
		bool _isRestricted;
		int _userLimit;
		std::string _password;
		std::string _topic;
};
