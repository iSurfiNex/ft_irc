#include "Gpt.hpp"

int Gpt::delayBetweenRequest = 5;
std::time_t Gpt::lastRequestTs = std::time(NULL);
int Gpt::maxRequestCount = 25;
int Gpt::requestCount = 0;
std::string Gpt::_apiKey;

Gpt::Gpt() {}

Gpt::~Gpt() {}

void Gpt::setApiKey(const std::string &apiKey) { _apiKey = _cleanStr(apiKey); }

void Gpt::setPreprompt(const std::string &preprompt) {
  _preprompt =
      _cleanStr(preprompt) +
      ". Answer a short answer in french, do not include chariage return "
      "character in your answer.";
}

bool Gpt::checkApiKey(void) { return !_apiKey.empty(); }

std::string Gpt::_parseJsonResponse(const std::string &result) {
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

std::string Gpt::_removeChars(const std::string &str,
                              const std::string &toReplace) {
  std::string result = str;
  size_t pos = 0;
  while ((pos = result.find_first_of(toReplace), pos) != std::string::npos) {
    result.replace(pos, 1, "");
  }
  return result;
}

std::string Gpt::_cleanStr(const std::string &str) {
  std::string result;
  result = _removeChars(str, "\r\n\'\"\\"); // prevents code injection
  return result;
}

std::string Gpt::_getGptRequestCommand(const std::string &apiKey,
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

std::string Gpt::_executeCommand(const std::string &command) {
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

std::string Gpt::getReply(const std::string msg) {
  if (requestCount > maxRequestCount) {
    return "Too many requests, restart me to continue";
  }
  std::time_t currentTime, threshold;
  while ((currentTime = std::time(NULL)) <
         (threshold = lastRequestTs + delayBetweenRequest)) {
    float remaining = difftime(threshold, currentTime);
    std::cout << "Too many requests, wait " << remaining
              << " seconds for request..." << std::endl;
    sleep(remaining);
  }
  lastRequestTs = std::time(NULL);
  requestCount++;
  std::string cleanMsg = _cleanStr(msg);
  std::string command = _getGptRequestCommand(_apiKey, cleanMsg, _preprompt);
  std::string jsonResponse = _executeCommand(command);
  std::string gptResponse = _parseJsonResponse(jsonResponse);
  if (gptResponse.empty()) {
    std::cerr << "Error: could not parse response: " << std::endl
              << jsonResponse << std::endl;
    std::cerr << "This was sent: " << std::endl << command << std::endl;
  }

  return gptResponse;
}
