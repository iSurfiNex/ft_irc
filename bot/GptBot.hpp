#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>

#include "Gpt.hpp"
#include "IrcClient.hpp"

class GptBot : public IrcClient, public Gpt {

public:
  /* LIFECYCLE */

  GptBot();
  ~GptBot();

  void _handleMsg(const std::string &msg);
};
