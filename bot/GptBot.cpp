#include "GptBot.hpp"

GptBot::GptBot() : IrcClient(), Gpt() {}

GptBot::~GptBot() {}

bool _handleApiKeyPrivMsg(const std::string &privMsg) {
  std::istringstream iss(privMsg);
  std::string token;

  if (iss >> token) {
    if (token == "APIKEY")
      if (iss >> token) {
        Gpt::setApiKey(token);
        return true;
      }
  }
  return false;
}

/* Gpt replies to the sender if msg is a well formatted PRIVMSG */
void GptBot::_handleMsg(const std::string &msg) {
  std::string sender = "";
  std::string privMsg = "";
  if (!_parsePrivMsg(msg, sender, privMsg))
    return;

  if (_handleApiKeyPrivMsg(privMsg)) {
    _sendPrivMsg(sender, "Ok j'écoute");
    return;
  }

  if (!checkApiKey()) {
    _sendPrivMsg(sender, "Commence par envoyer une clé d'API: "
                         "\"APIKEY ta_clé_dAPI\" par MP.");
    return;
  }

  std::string gptResponse = getReply(privMsg);
  if (gptResponse.empty())
    gptResponse =
        "Une erreur est survenu, je n'ai pas de réponse à proposer. Je te "
        "propose de caresser un poney en attendant.";
  _sendPrivMsg(sender, gptResponse);
}
