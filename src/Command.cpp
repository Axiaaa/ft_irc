#include "Server.hpp"


/*
    @brief Send a NICK command to the server
    @param server The server object
    @param client_fd The client file descriptor
    @param buffer The buffer containing the nickname
*/
void nick(Server &server, int client_fd, const char *buffer)
{
    std::string nick = "NICK ";
    nick += buffer;
    nick += "\r\n";
    server.sendData(client_fd, nick);
}

/*
    @brief Send a USER command to the server
    @param server The server object
    @param client_fd The client file descriptor
    @param buffer The buffer containing the username
*/
void user(Server &server, int client_fd, const char *buffer)
{   
    std::string user = "USER ";
    user += buffer;
    user += "\r\n";
    server.sendData(client_fd, user);
}