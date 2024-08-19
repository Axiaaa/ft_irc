#include "Server.hpp"

void cap(Server &server, int client_fd, const char *buffer)
{
    (void)buffer;
    std::string cap = "CAP * LS :multi-prefix sasl\r\n";
    send(client_fd, cap.c_str(), cap.size(), 0);
    (void)server;
}

void nick(Server &server, int client_fd, const char *buffer)
{
    (void)server;
    //Create a basic NICK command
    std::string nick = "NICK ";
    nick += buffer;
    nick += "\r\n";
    send(client_fd, nick.c_str(), nick.size(), 0);
}

void user(Server &server, int client_fd, const char *buffer)
{   
    (void)server;
    //Create a basic USER command
    std::string user = "USER ";
    user += buffer;
    user += "\r\n";
    send(client_fd, user.c_str(), user.size(), 0);
}