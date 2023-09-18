#pragma once

#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <signal.h>
#include <sstream>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

#define BUFFER_SIZE 512
#define MAX_CONNECT_ATTEMPT 30
#define CONNECT_RETRY_DELAY 5

class IrcClient {
public:
  /* LIFECYCLE */

  IrcClient(); // const std::string &address, const std::string &port);
  virtual ~IrcClient();

  /* GET SET */

  void setChannel(const std::string &chan);

  /* ACTIONS */

  void receiveRespondLoop();
  bool tryConnectToServerLoop(const std::string &ip, int port);
  bool connectToServer(const std::string &ip, int port);
  bool sendMessage(const std::string &msg) const;
  bool authenticate(const std::string &serverPw, const std::string &user,
                    const std::string &nick) const;
  void joinChanIfNeeded();

protected:
  /* ACTIONS  */

  bool _parsePrivMsg(const std::string &msg, std::string &sender,
                     std::string &privMsg) const;
  void _sendPrivMsg(const std::string &nick, const std::string &msg) const;
  virtual void _handleMsg(const std::string &msg) = 0;

private:
  /* ATTRIBUTES */

  std::string _address;
  std::string _port;
  std::string _chan;
  int _sd;
  int _connectAttempt;
  struct sockaddr_in _serverAddress;
};
