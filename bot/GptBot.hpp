#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

#include "Gpt.hpp"

class GptBot : public Gpt {

public:
  /* LIFECYCLE */

  GptBot(const std::string &address, const std::string &port);
  ~GptBot();

private:
  /* ATTRIBUTES */

  std::string _address;
  std::string _port;
};
