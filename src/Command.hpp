#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Server.hpp"

class Server;
class Client;

void	nick(Server& server, Client& client, const string &buffer);
void	user(Server& server, Client& client, const string &buffer);
void	privmsg(Server& server, Client& client, const string &buffer);
void	join(Server &server, Client &client, const string &buffer);
void	who(Server &server, Client &client, const string &buffer);
void	mode(Server &server, Client &client, const string &buffer);
void	pass(Server& server, Client& client, const string &buffer);
void	topic(Server& server, Client& client, const string &buffer);
void	invite(Server& server, Client& client, const string &buffer);
void	kick(Server& server, Client& client, const string &buffer);
void	part(Server& server, Client& client, const string &buffer);
void	pong(Server& server, Client& client, const string &buffer);
void	motd(Server& server, Client& client);
void    quit(Server& server, Client& client, const string &buffer);
void    list(Server& server, Client& client, const string &buffer);

#endif