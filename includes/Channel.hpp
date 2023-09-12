/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 19:55:19 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/12 14:15:19 by rsterin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "IrcServer.hpp"

class Channel
{
	public:
		Channel(const std::string cName);
		~Channel(void);

		std::string channelname;

		std::set<std::string> userList;
		std::set<std::string> modList;

		bool isInviteOnly;
		bool isTopicChangeable;
		bool isRestricted;
		int userLimit;
};
