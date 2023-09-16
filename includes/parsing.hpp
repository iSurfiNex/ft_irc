#pragma once

#include "IrcServer.hpp"

int parsePort(char *av);
std::string parsePassword(char *av);
void getCmdArgs(std::string buffer, std::string &cmd, strVec_t &args);

void cmdPass(strVec_t &args, Client &origin, IrcServer &server);
void cmdUser(strVec_t &args, Client &origin, IrcServer &server);
void cmdNick(strVec_t &args, Client &origin, IrcServer &server);
void cmdPrivMsg(strVec_t &args, Client &origin, IrcServer &server);
void cmdPart(strVec_t &args, Client &origin, IrcServer &server);
void cmdInvite(strVec_t &args, Client &origin, IrcServer &server);
void cmdTopic(strVec_t &args, Client &origin, IrcServer &server);
void cmdKick(strVec_t &args, Client &origin, IrcServer &server);
void cmdMode(strVec_t &args, Client &origin, IrcServer &server);
void cmdJoin(strVec_t args, Client &client, IrcServer &server);

void parsing(Client &origin, IrcServer &server, std::string buffer);
