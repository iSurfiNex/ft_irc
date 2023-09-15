#include "IrcServer.hpp"

void cmdJoin(std::vector<std::string> args, Client &client, IrcServer &server) {
  int argc = args.size();
  if (argc == 0)
    client.msg(ERR_NEEDMOREPARAMS, "JOIN");
  std::vector<std::string> chans;
  std::vector<std::string> keys;
  for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++) {
    std::string entry = *it;

    if (Channel::isValidName(entry))
    {
        if (std::find(chans.begin(), chans.end(), entry) != chans.end())
        {
            client.msg(ERR_BADCHANNELMASK, entry, "Bad Channel Mask, same channel provided multiple times");
            return;
        }
        chans.push_back(entry);
    }else if (Channel::isValidKey(entry))
        chans.push_back(entry);
     else
         client.msg(ERR_BADCHANNELMASK, entry, "Bad Channel Mask");
  }

    if (chans.size() < keys.size())
    {
        std::string erroredEntry = keys[chans.size()];
        client.msg(ERR_BADCHANNELMASK, erroredEntry, "Bad Channel Mask");
    }
    (void)server;

    //for (std::vector<std::string>::iterator it = chans.begin(); it != chans.end();
    //{
    //    std::string chanName = *it;
    //    server.createChannel();
    //}


  // Channel *channel = server.getChannelWithName(channelName);
  // if (!channel)
  //   server.createChannel(channelName, client);
  // else if (channel->isInviteOnly && !channel->isUserOnInviteList(client))
  //   return "You need an invitation to join channel " + channelName + "\r\n";
  // else if (channel->isRestricted) {
  //   if (!pwProvided)
  //     return "Channel " + channelName + "requires a password" + "\r\n";
  //   std::string &pw = args[1];
  //   if (!channel->checkPassword(pw))
  //     return "Wrong password for Channel " + channelName + "\r\n";
  // }
  // channel->addUser(client);
  // return "You have join channel " + channelName + "\r\n";
}
