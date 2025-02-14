/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   typedef.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 18:49:40 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/17 22:54:59 by rsterin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

class Channel;
class Client;
class IrcServer;

typedef std::vector<std::string> strVec_t;
typedef std::vector<int> intVec_t;
typedef std::set<Channel *> chanSet_t;
typedef std::set<Client *> clientSet_t;
typedef std::set<const Client *> constClientSet_t;
typedef std::string str_t;

enum msgCode_e {
  RPL_WELCOME = 001,
  RPL_NOTOPIC = 331,
  RPL_TOPIC = 332,
  RPL_INVITING = 341,
  RPL_NAMREPLY = 353,
  RPL_ENDOFNAMES = 366, // ?
  ERR_NOSUCHNICK = 401,
  ERR_NOSUCHCHANNEL = 403,
  ERR_CANNOTSENDTOCHAN = 404,
  ERR_TOOMANYCHANNELS = 405,
  ERR_NOTEXTTOSEND = 412,
  ERR_NONICKNAMEGIVEN = 431,
  ERR_ERRONEUSNICKNAME = 432,
  ERR_NICKNAMEINUSE = 433,
  ERR_USERNOTINCHANNEL = 441,
  ERR_NOTONCHANNEL = 442,
  ERR_USERONCHANNEL = 443,
  ERR_NEEDMOREPARAMS = 461,
  ERR_ALREADYREGISTERED = 462,
  ERR_PASSWDMISMATCH = 464,
  ERR_CHANNELISFULL = 471,
  ERR_INVITEONLYCHAN = 473,
  ERR_BADCHANNELKEY = 475,
  ERR_BADCHANNELMASK = 476,
  ERR_CHANOPRIVSNEEDED = 482,
  MSG_JOIN = 1000,
  MSG_ADDINVITELIST = 1001,
  MSG_MODE = 1003,
  ERR_NOTVALIDENTRY = 1004,
  MSG_PART = 1005,
  ERR_USERLIMIT = 1006,
};
