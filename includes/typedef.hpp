#pragma once

typedef std::vector<std::string> strVec_t;
typedef std::string str_t;

enum msgCode_e {
  RPL_TOPIC = 332,
  RPL_NAMREPLY = 353,
  RPL_ENDOFNAMES = 366, // ?
  ERR_NOSUCHCHANNEL = 403,
  ERR_TOOMANYCHANNELS = 405,
  ERR_NEEDMOREPARAMS = 461,
  ERR_CHANNELISFULL = 471,
  ERR_INVITEONLYCHAN = 473,
  ERR_BADCHANNELKEY = 475,
  ERR_BADCHANNELMASK = 476,
  MSG_JOIN = 1000
};
