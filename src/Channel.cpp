/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 19:55:29 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/12 14:17:05 by rsterin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(const std::string cName)
{
	channelname = cName;

	std::cout << "New channel: " << channelname << std::endl;

	isInviteOnly = false;
	isTopicChangeable = false;
	isRestricted = false;
	userLimit = false;
}

Channel::~Channel(void)
{

}
