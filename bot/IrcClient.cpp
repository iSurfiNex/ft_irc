#include "IrcClient.hpp"

IrcClient::IrcClient() : _chan(""), _sd(-1), _connectAttempt(0) {}

IrcClient::~IrcClient() {
  if (_sd != -1)
    close(_sd);
}

bool IrcClient::authenticate(const std::string &serverPw,
                             const std::string &user,
                             const std::string &nick) const {
  return sendMessage("PASS " + serverPw + "\r\n") &&
         sendMessage("USER " + user + "\r\n") &&
         sendMessage("NICK " + nick + "\r\n");
}

void IrcClient::setChannel(const std::string &chan) { this->_chan = chan; }

void IrcClient::joinChanIfNeeded() {
  std::cout << "CHAN!!!" << this->_chan << std::endl;
  if (!_chan.empty())
    sendMessage("JOIN " + _chan + "\r\n");
}

/* Listen to incomming privmsg and respond to the snder with gpt. */
void IrcClient::receiveRespondLoop() {
  char buffer[BUFFER_SIZE];
  while (true) {
    buffer[0] = '\0';
    int bytesRead = recv(_sd, buffer, sizeof(buffer) - 1, 0);

    if (bytesRead == -1) {
      std::cerr << "Error receiving data" << std::endl;
      break;
    } else if (bytesRead == 0) {
      std::cout << "Connection closed by the server." << std::endl;
      break;
    } else {
      buffer[bytesRead] = '\0';
      std::string msg = buffer;
      std::cout << "Msg received: " << msg << std::endl;
      _handleMsg(msg);
    }
  }
}

/* Return false if msg was not a well formatted privmsg. Populate sender and
 * privMsg. */
bool IrcClient::_parsePrivMsg(const std::string &msg, std::string &sender,
                              std::string &privMsg) const {
  std::cout << msg << "!!!";
  std::istringstream iss(msg);
  if (msg.size() > 0 && msg[0] == ':')
    iss.ignore(1);

  std::vector<std::string> tokens;

  std::string token;
  while (iss >> token) {
    if (token[0] == ':') {
      // Set to privmsg to anything after ":" in the stream
      privMsg =
          token.substr(1, std::string::npos) + iss.str().substr(iss.tellg());
      break;
    } else {
      tokens.push_back(token);
    }
  }

  if (privMsg.empty() || tokens.size() != 3 || tokens[1] != "PRIVMSG")
    return false;

  if (tokens[2][0] == '#') // channel msg
    sender = tokens[2];    // respond to channel
  else
    sender = tokens[0]; // respond to client

  std::cout << "SENDER: " << sender << std::endl;

  return true;
}

/* Send privmsg to nick with msg. */
void IrcClient::_sendPrivMsg(const std::string &nick,
                             const std::string &msg) const {
  std::string formattedMsg = "PRIVMSG " + nick + " :" + msg + "\r\n";
  sendMessage(formattedMsg);
}

/* Send msg to the server */
bool IrcClient::sendMessage(const std::string &msg) const {
  const char *msg_cstr = msg.c_str();
  if (send(_sd, msg_cstr, strlen(msg_cstr), 0) == -1) {
    std::cerr << "Failed to send data" << std::endl;
    return false;
  }
  return true;
}

bool IrcClient::tryConnectToServerLoop(const std::string &ip, int port) {

  for (_connectAttempt = 1; _connectAttempt <= MAX_CONNECT_ATTEMPT;
       _connectAttempt++) {
    if (connectToServer(ip, port))
      return true;
    std::cerr << "Retrying in " << CONNECT_RETRY_DELAY << " seconds"
              << std::endl;
    sleep(CONNECT_RETRY_DELAY);
  }
  std::cerr << "Max connection attempt reached (" << MAX_CONNECT_ATTEMPT
            << "). Aborting." << std::endl;
  return false;
}

bool IrcClient::connectToServer(const std::string &ip, int port) {
  const char *ip_cstr = ip.c_str();

  // Create a socket
  _sd = socket(AF_INET, SOCK_STREAM, 0);
  if (_sd == -1) {
    std::cerr << "Failed to create socket" << std::endl;
    return false;
  }

  // Set up the server address structure
  struct sockaddr_in _serverAddress;
  _serverAddress.sin_family = AF_INET;
  _serverAddress.sin_port = htons(port);
  _serverAddress.sin_addr.s_addr = inet_addr(ip_cstr);

  // Connect to the server
  if (connect(_sd, (struct sockaddr *)&_serverAddress,
              sizeof(_serverAddress)) == -1) {
    std::cerr << "Failed to connect to the server at " << ip << ":" << port
              << " [attempt " << _connectAttempt << "/" << MAX_CONNECT_ATTEMPT
              << "]" << std::endl;
    return false;
  }

  return true;
}
