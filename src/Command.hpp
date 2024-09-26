#pragma once

#include "Server.hpp"

class Server;
class Client;

void	nick(Server& server, Client& client, const string &buffer);
void	user(Server& server, Client& client, const string &buffer);
void	privmsg(Server& server, Client& client, const string &buffer);
void	join(Server &server, Client &client, const string &buffer);
void	who(Server &server, Client &client, const string &buffer);
void	mode(Server &server, Client &client, const string &buffer);
void    pass(Server& server, Client& client, const string &buffer);
void    topic(Server& server, Client& client, const string &buffer);
