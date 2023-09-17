#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

class GptBot {

public:
  /* LIFECYCLE */

  GptBot(const std::string &address, const std::string &port);
  ~GptBot();

  /* ACTIONS */

  std::string getReply(const std::string msg);

  /* GET/SET */

  void setApiKey(const std::string &apiKey);
  void setPreprompt(const std::string &apiKey);

private:
  /* ACTIONS */

  static std::string _executeCommand(const std::string &command);
  static std::string _parseJsonResponse(const std::string &result);
  static std::string _removeChars(const std::string &str,
                                  const std::string &toReplace);
  static std::string _cleanStr(const std::string &str);
  static std::string _getGptRequestCommand(const std::string &apiKey,
                                           const std::string &msg,
                                           const std::string &preprompt);
  /* ATTRIBUTES */

  std::string _apiKey;
  std::string _preprompt;
  std::string _address;
  std::string _port;
};
