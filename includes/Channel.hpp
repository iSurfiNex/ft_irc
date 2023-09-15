/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 19:55:19 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/14 18:14:00 by rsterin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "IrcServer.hpp"

class Client;

class Channel
{
	public:
		Channel(const std::string &_name, const std::string &key, Client &mod);
		~Channel(void);

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

		bool isMod(const Client &client);
		bool isUserInside(const Client &client);

		bool isUserOnInviteList(const Client &client);
		bool checkPassword(const std::string &pw);

		void sendMessage(const std::string message) const;

		std::string name;

		bool isInviteOnly;
		bool isTopicChangeable;
		bool isRestricted;
		static bool isValidKey(const std::string &key);
		static bool isValidName(const std::string &name);
		std::string serverName;
		void msg(msgCode_e code, ...) const;
		void tryEnter(Client &client, const std::string& password);
	private:

		static bool _isValidBaseName(const std::string &name);
		static bool _isValidNamePrefix(char c);

		static const std::string _allowedNamePrefix;
		static const std::string _forbiddenNameChars;
		static const int _nameMaxLen;

		std::string _getUserListStr(void) const;

		std::set<const Client *> _userList;
		std::set<const Client *> _inviteList;
		std::set<const Client *> _modList;

		int _userLimit;
		std::string _password;
		std::string _topic;
		std::string _symbol;

};
