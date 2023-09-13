#include "IrcServer.hpp"

#define USAGE "JOIN <channel> (<password>)"
#define ARGC_ERR "JOIN arg count error: " USAGE
#define CHANNEL_NAME_MAX_SIZE 50
#define ERR_CHANNEL_MAX_SIZE "The channel name max size is ##CHANNEL_NAME_MAX_SIZE"

const std::string cmdJoin(std::vector<std::string> args, Client &client, IrcServer &server) {
    int argc = args.size();
    if (argc == 0 || argc > 2)
       return ARGC_ERR;

    std::string channelName = args[0];
    int channelNameSize = channelName.size();
    if (channelNameSize == 0 || channelName == "#")
        return "The channel name provided is empty.\r\n";
    if (channelName[0] != '#')
        return "The channel name must start with a '#'.\r\n";
    std::string nameBase = channelName;
    nameBase.erase(0, 1);
    if (!str_alnum(nameBase))
        return "The channel name must be alphanumeric.\r\n";
    if (channelName.size() > CHANNEL_NAME_MAX_SIZE)
        return ERR_CHANNEL_MAX_SIZE ".\r\n";

    bool pwProvided = argc == 2;
    Channel *channel = server.getChannelWithName(channelName);
    if (!channel)
        server.createChannel(channelName, client);
    else if (channel->isInviteOnly && !channel->isUserOnInviteList(client))
        return "You need an invitation to join channel " + channelName +"\r\n";
    else if (channel->isRestricted)
    {
        if( !pwProvided)
            return "Channel " + channelName + "requires a password"+"\r\n";
        std::string &pw = args[1];
        if (!channel->checkPassword(pw))
            return "Wrong password for Channel " + channelName +"\r\n";
    }
    channel->addUser(client);
    return "You have join channel "  + channelName+"\r\n";
}
