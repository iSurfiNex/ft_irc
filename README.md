<h1 align="center">ft_irc</h1>

<p align="center">
	<b><i>This is a 42school's project that is about creating an IRC server in C++</i></b><br>
</p>
<p align="center">
	<img alt="GitHub repo size" src="https://img.shields.io/github/repo-size/iSurfiNex/ft_irc?color=lightblue&style=for-the-badge">
	<img alt="GitHub commit activity" src="https://img.shields.io/github/commit-activity/t/iSurfiNex/ft_irc?color=critical&style=for-the-badge">
	<img alt="Code language count" src="https://img.shields.io/github/languages/count/iSurfiNex/ft_irc?color=yellow&style=for-the-badge"/>
	<img alt="GitHub top language" src="https://img.shields.io/github/languages/top/iSurfiNex/ft_irc?color=blue&style=for-the-badge"/>
	<img alt="GitHub last commit" src="https://img.shields.io/github/last-commit/iSurfiNex/ft_irc?color=green&style=for-the-badge"/>
</p>
<div align="center">
  <a href="https://github.com/Coday-meric/badge42"><img src="https://badge42.coday.fr/api/v2/clt38xgf41137601p4ax754vzt/project/3304315" alt="rsterin's 42 ft_irc Score" /></a>
</div>

# Table of Contents

- [Information](#information)
- [Installation](#installation)
- [Usage](#usage)

# Information

As this project is about creating an IRC server, we had to develop a server that enable people talking to each other through private messages and channels.
We use HexChat as main client but it's working with NetCat too. 

These are some required features we had to do :
- Being able to authentificate, set a nickname, set a username.
- Operators and regular users.
- Implement commands such as : KICK, INVITE, TOPIC, MODE.
- Handle Ctrl+D (it send the messages in serveral parts).

This project has a bonus part and we had done it, this is the required features :
- Handle file transfer.
- A bot.

# Installation

In order to compile the project, please use this command :

```bash
make
```
</br>

And for the bonus (bot) :
```bash
make bonus
```

# Usage

To start the server, please use this command :

```bash
./ircserv <PORT> <PASSWORD>
```
</br>

And for the bonus (bot) :
```bash
./gptbot <IP> <PORT> <PASSWORD> <BOTNAME> [--apiKey=xxxxx] [--preprompt=Some]
```
