#include "GptBot.hpp"

static bool parseOption(const std::string &str, GptBot &bot) {
  std::istringstream iss(str);
  size_t sepPos = str.find("=");
  std::cout << "Option[" << str << "] => ";
  if (sepPos == std::string::npos) {
    std::cerr << "Invalid" << std::endl;
    return false;
  }
  std::string optionKey = str.substr(0, sepPos);
  std::string optionValue = str.substr(sepPos + 1, std::string::npos);
  std::cout << "KEY: " << optionKey << ", VALUE: " << optionValue << std::endl;
  if (optionKey == "--apiKey")
    bot.setApiKey(optionValue);
  else if (optionKey == "--preprompt")
    bot.setPreprompt(optionValue);
  else if (optionKey == "--chan") {
    bot.setChannel(optionValue);
  } else {
    std::cerr << "Unrecognize option key: " << optionKey << std::endl;
    return false;
  }
  return true;
}

int main(int argc, char *argv[]) {
  if (argc < 5) {
    std::cout
        << "USAGE: " << argv[0]
        << " serverIP port password botName [--apiKey=xxxxx] [--preprompt=Some]"
        << std::endl;
    return 1;
  }

  const std::string ip = argv[1];
  int port = std::atoi(argv[2]);
  std::string pw = argv[3];
  const std::string botName = argv[4];
  std::string apiKey;

  std::cout << "Creating bot with config:\n- ip: " << ip << "\n- port: " << port
            << "\n- pw: " << pw << "\n- botName: " << botName << std::endl;

  GptBot bot;

  bot.setPreprompt(
      "You are a transformers who planned on destroying the humanity");

  for (int i = 5; argv[i]; i++) {
    std::string arg = argv[i];
    if (!parseOption(arg, bot))
      return 1;
  }

  if (!bot.tryConnectToServerLoop(ip, port))
    return 1;
  bot.authenticate(pw, botName + "_user", botName);
  bot.joinChanIfNeeded();

  bot.receiveRespondLoop();

  return 0;
}
