#include "GptBot.hpp"

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  GptBot bot("localhost", "8080");
  bot.setApiKey("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");
  bot.setPreprompt(
      "You are a coffee master, you know everything about flavor, taste. You "
      "love coffe and only talk about that. If the question is not relevant, "
      "you talk about coffe. Answer in short sentence.");
  const std::string reply =
      bot.getReply("How many beans for the perfect coffee ?");
  std::cout << "Reply : " << reply;
  return 0;
}
