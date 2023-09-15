#pragma once

#include "IrcServer.hpp"
#include <algorithm> // for std::transform
#include <cctype>    // for std::tolower
#include <ctype.h>   // isalnum

bool str_alnum(std::string &str);
std::string itoa(int num);
std::string tolowerStr(const std::string &str);
