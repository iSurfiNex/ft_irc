#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <string>
#include <unistd.h>

class Gpt {

public:
  /* LIFECYCLE */

  Gpt();
  virtual ~Gpt();

  /* ACTIONS */

  std::string getReply(const std::string msg);

  /* CHECK */

  static bool checkApiKey(void);

  /* GET/SET */

  static void setApiKey(const std::string &apiKey);
  void setPreprompt(const std::string &preprompt);

protected:
  static std::string _apiKey;

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

  std::string _preprompt;

  static int delayBetweenRequest;
  static std::time_t lastRequestTs;
  static int maxRequestCount;
  static int requestCount;
};
