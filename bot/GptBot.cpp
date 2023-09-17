#include "GptBot.hpp"

GptBot::GptBot(const std::string &address, const std::string &port)
    : Gpt(), _address(address), _port(port) {}

GptBot::~GptBot() {}
