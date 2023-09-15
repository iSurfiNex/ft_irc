#include "IrcServer.hpp"

void cmdJoin(strVec_t args, Client &client, IrcServer &server) {
  int argc = args.size();
  if (argc == 0)
    client.msg(ERR_NEEDMOREPARAMS, "JOIN");
  strVec_t chans;
  strVec_t keys;
  foreach(strVec_t, args)
  {
    std::string entry = *it;

    if (Channel::isValidName(entry))
    {
        std::string entryLower = tolowerStr(entry);
        if (std::find(chans.begin(), chans.end(), entryLower) != chans.end())
        {
            client.msg(ERR_BADCHANNELMASK, entry, "Bad Channel Mask, same channel name provided multiple times");
            return;
        }
        chans.push_back(entryLower);
    }else if (Channel::isValidKey(entry))
        keys.push_back(entry);
     else
         client.msg(ERR_BADCHANNELMASK, entry, "Bad Channel Mask");
  }

    if (chans.size() < keys.size())
    {
        std::string erroredEntry = keys[chans.size()];
        client.msg(ERR_BADCHANNELMASK, erroredEntry, "Bad Channel Mask");
    }

    size_t i = 0;
    foreach(strVec_t, chans)
    {
        std::string chanName = *it;
        Channel *channel = server.getChannelWithName(chanName);
        std::string key = "";
        if (i < keys.size())
          key = keys[i];
        if (!channel)
          server.createChannel(client, chanName, key);
        else
          channel->tryEnter(client, key);
        i++;
    }
}
