/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 19:55:19 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/12 14:37:18 by rsterin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "IrcServer.hpp"

class Channel
{
	public:
		Channel(const std::string cName);
		~Channel(void);

		void addUser(const std::string username);
		void addMod(const std::string username);

		void removeUser(const std::string username);
		void removeMod(const std::string username);

		void changeRule(const std::string username, const char mode);
		void changePassword(const std::string username, const std::string password);
		void changeUserLimit(const std::string username, const int userlimit);

		bool isMod(const std::string username);

		std::string channelname;

	private:
		std::set<std::string> userList;
		std::set<std::string> modList;

		bool _isInviteOnly;
		bool _isTopicChangeable;
		bool _isRestricted;
		int _userLimit;
		std::string _password;
		std::string _topic;
};
