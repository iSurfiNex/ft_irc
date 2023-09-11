/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 19:55:19 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/11 20:07:45 by rsterin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "IrcServer.hpp"

class Channel
{
	public:
		// Channel(void);
		Channel(const std::string username, const std::string nickname);
		~Channel(void);

		std::set<std::string> userList;
		std::set<std::string> modList;

		bool isInviteOnly;
		bool isTopicChangeable;
		bool isRestricted;
		int userLimit;
};
