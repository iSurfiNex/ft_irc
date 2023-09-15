#pragma once

typedef std::vector<std::string> strVec_t;
typedef std::string str_t;

enum msgCode_e {
  ERR_YO = 123,
  RPL_NOTOPIC = 331,
  RPL_TOPIC = 332,
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
  ERR_YA = 444,
  ERR_NEEDMOREPARAMS = 461,
  ERR_ALREADYREGISTERED = 462,
  ERR_PASSWDMISMATCH = 464,
  ERR_CHANNELISFULL = 471,
  ERR_INVITEONLYCHAN = 473,
  ERR_BADCHANNELKEY = 475,
  ERR_BADCHANNELMASK = 476,
  ERR_CHANOPRIVSNEEDED = 482,
};
