#include "Server.hpp"


void cap(Server &server, int client_fd, const char *buffer);
void nick(Server &server, int client_fd, const char *buffer);
void user(Server &server, int client_fd, const char *buffer);
