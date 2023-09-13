- JOIN <channel> (<password>)
- PART <channel>
- PRIVMSG <nickname>/<channel> : <msg>
- KICK <channel> <nickname> (<reason>)
- INVITE <nickname> <channel>
- TOPIC <channel> : <msg>
- MODE <channel> <param> (<msg>)

Param: i, t, k, o, l

Utilisateur: doit avoir un user/nick isalnum;

PARSING: R  <!-- done -->
	cmd = determine la cmd;
	args = trouver les args (gerer les deux points);

	remplir switch case et creer les fonctions pour chaque commande;

	verif si le user isReady return si non;

__________________________________________

PASS/USER/NICK: R
	verifier bon mdp;
	verifier user/nick deja pris
	changer isAuth a true;
__________________________________________

JOIN: F
	verifier qu'il n'y ai pas plus de 1/2 args;
	creer un channel si existe pas;
	si existe -> verifier si isRestricted ||&& isInviteOnly ||&& nbuser + 1 <= userLimit;
	rejoindre le channel;
__________________________________________

PART: F
	verifier qu'il n'y ai pas plus de 1 args;
	verifier si channel existe si oui leave + l'enlever des mods;
__________________________________________

PRIVMSG: R
	verifier si le channel/user existe;
	verifier si l'utilisateur est dans le channel;
	verifier qu'il n'y ai pas plus de 2 args;
	envoyer l'arg 2 a la target;
__________________________________________

KICK: F
	verifier si le channel existe;
	verifier si la target existe;
	verifier si la target est deja dans le channel;
	verifier si la target n'est pas mod;
	verifier si la target n'est pas l'auteur de la cmd;
	verifier que l'utilisateur soit mod;
	verifier si l'utilisateur est dans le channel;
	verifier qu'il n'y ai pas plus de 2/3 args;
	removeUser la target du channel + envoyer
__________________________________________

INVITE: R
	verifier si le channel existe;
	verifier si la target existe;
	verifier si la target n'est pas deja dans le channel;
	verifier si la target n'est pas l'auteur de la cmd;
	verifier que l'utilisateur soit mod;
	verifier si l'utilisateur est dans le channel;
	verifier qu'il n'y ai pas plus de 2 args;
	invite la target dans le channel;
__________________________________________

TOPIC: F
	verifier si le channel existe;
	verifier si l'utilisateur est dans le channel;
	verifier que l'utilisateur soit mod seulement is isTopicChangeable = false;
	verifier qu'il n'y ai pas plus de 2 args;
	changer
__________________________________________

MODE: F
	verifier si le channel existe;
	verifier que l'utilisateur soit mod;
	verifier que le parametre existe;
	verifier que le msg soit present si requis (pour le password, pour le operator ou pour le userlimit);
	verifier qu'il n'y ai pas plus de 2/3 args;
	appliquer le parametre au channel;



