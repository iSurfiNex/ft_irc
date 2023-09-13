#include "IrcServer.hpp"

#define USAGE "JOIN <channel> (<password>)"
#define ARGC_ERR "JOIN arg count error: " USAGE

const std::string cmdJoin(const std::string &cmd, std::vector<std::string> args, Client &client, IrcServer &server) {
    int argc = args.size();
    if (argc == 0 || argc > 2)
       return ARGC_ERR;

    std::string channelName = args[0];
    bool pwProvided = argc == 2;
    Channel *channel = server.getChannelWithName(channelName);
    if (!channel)
        server.createChannel(channelName, client);
    else if (channel->isInviteOnly && !channel->isUserOnInviteList(client))
        return "You need an invitation to join channel " + channelName +"\r\n";
    else if (channel->isRestricted)
        if( !pwProvided)
            return "Channel " + channelName + "requires a password"+"\r\n";
        std::string &pw = args[1];
        if (!channel->checkPassword(pw))
            return "Wrong password for Channel " + channelName +"\r\n";
    channel->addUser(client);
    return "You have join channel "  + channelName+"\r\n";
}
