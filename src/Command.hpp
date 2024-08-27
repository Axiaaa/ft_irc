#pragma once

#include "Server.hpp"

void nick(Server& server, Client& client, const char *buffer);
void user(Server& server, Client& client, const char *buffer);
void privmsg(Server& server, Client& client, const char *buffer);
void pass(Server& server, Client& client, const char *buffer);
