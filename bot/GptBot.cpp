#include "GptBot.hpp"

GptBot::GptBot(const std::string &address, const std::string &port)
    : _address(address), _port(port) {}

GptBot::~GptBot() {}

void GptBot::setApiKey(const std::string &apiKey) {
  _apiKey = _cleanStr(apiKey);
}

void GptBot::setPreprompt(const std::string &preprompt) {
  _preprompt = _cleanStr(preprompt);
}

std::string GptBot::_parseJsonResponse(const std::string &result) {
  size_t startPos = result.find("\"content\": \"");
  if (startPos == std::string::npos)
    return "";
  startPos += 12;
  size_t endPos = result.find("\"\n      },", startPos);
  if (endPos == std::string::npos)
    return "";
  size_t len = endPos - startPos;
  std::string msg = result.substr(startPos, len);
  return msg;
}

std::string GptBot::_removeChars(const std::string &str,
                                 const std::string &toReplace) {
  std::string result = str;
  size_t pos = 0;
  while ((pos = result.find_first_of(toReplace), pos) != std::string::npos) {
    result.replace(pos, 1, "");
    std::cout << result << std::endl;
  }
  return result;
}

std::string GptBot::_cleanStr(const std::string &str) {
  std::string result;
  result = _removeChars(
      str, "\'\"\\"); // remove ',",\ character to prevent code injection
  return result;
}

std::string GptBot::_getGptRequestCommand(const std::string &apiKey,
                                          const std::string &msg,
                                          const std::string &preprompt) {
  const char *gptUrl = "https://api.openai.com/v1/chat/completions";

  std::string request = "curl -X POST -H \"Authorization: Bearer ";
  request += apiKey;
  request += "\" -H \"Content-Type: application/json\" -d '";
  request += "{\"model\": \"gpt-3.5-turbo-0613\", \"messages\": [{\"role\": "
             "\"system\", \"content\": \"" +
             preprompt + "\"}, ";
  request += "{\"role\": \"user\", \"content\": \"" + msg + "\"} ] }' ";
  request += gptUrl;
  return request;
}

std::string GptBot::_executeCommand(const std::string &command) {
  FILE *pipe = popen(command.c_str(), "r");
  if (!pipe) {
    std::cerr << "Error: Failed to open pipe." << std::endl;
    return "";
  }

  char buffer[128];
  std::string result;

  while (!feof(pipe)) {
    if (fgets(buffer, sizeof(buffer), pipe) != NULL) {
      result += buffer;
    }
  }

  pclose(pipe);

  return result;
}

std::string GptBot::getReply(const std::string msg) {
  std::string cleanMsg = _cleanStr(msg);
  std::string command = _getGptRequestCommand(_apiKey, cleanMsg, _preprompt);
  std::string jsonResponse = _executeCommand(command);
  std::string gptResponse = _parseJsonResponse(jsonResponse);
  return gptResponse;
}
