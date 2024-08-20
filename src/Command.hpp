#include "Server.hpp"


void nick(Server &server, int client_fd, const char *buffer);
void user(Server &server, int client_fd, const char *buffer);
