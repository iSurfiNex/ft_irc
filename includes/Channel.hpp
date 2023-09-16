/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 19:55:19 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/15 18:46:20 by rsterin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "IrcServer.hpp"

class Client;

class Channel
{
	public:

		/* LIFECYCLE */

		Channel(const std::string &_name, const std::string &key, Client &mod);
		~Channel(void);

		/* ACTIONS */

		void sendMessage(const std::string message) const;
		void msg(msgCode_e code, ...) const;
		void tryEnter(Client &client, const std::string& password);

		/* GET SET */

		void _addUser(const Client &client);
		void addUserInviteList(const Client &client);
		void addMod(const Client &client);

		void removeUser(const Client &client);
		void removeMod(const Client &client);
		void removeUserFromInviteList(const Client &client);

		void changeRule(const char mode);
		void changeTopic(const std::string topic);
		void changePassword(const std::string password);
		void changeUserLimit(const int userlimit);

		const std::string _getClientPrefix(const Client &client) const;

		/* CHECK */

		bool isMod(const Client &client) const;
		bool isUserInside(const Client &client) const;
		bool isUserOnInviteList(const Client &client) const;
		bool checkPassword(const std::string &pw) const;

		/* STATIC CHECK */

		static bool isValidKey(const std::string &key);
		static bool isValidName(const std::string &name);

		/* ATTRIBUTES */

		std::string name;
		std::string topic;

		bool isInviteOnly;
		bool isTopicChangeable;
		bool isRestricted;

		std::string serverName;

	private:

		/* CHECK */

		static bool _isValidBaseName(const std::string &name);
		static bool _isValidNamePrefix(char c);

		/* GET SET */

		std::string _getUserListStr(void) const;

		/* STATIC ATTRIBUTES */

		static const std::string _allowedNamePrefix;
		static const std::string _forbiddenNameChars;
		static const int _nameMaxLen;

		/* ATTRIBUTES */

		constClientSet_t _userList;
		constClientSet_t _inviteList;
		constClientSet_t _modList;

		int _userLimit;

		std::string _password;
		std::string _symbol;
};
