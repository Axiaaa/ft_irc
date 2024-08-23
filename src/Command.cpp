#include "Server.hpp"
#include "NumericReplies.cpp"
#include "Utils.hpp"

/*
    @brief Send a NICK command to the server
    @param server The server object
    @param client_fd The client file descriptor
    @param buffer The buffer containing the nickname
*/
void nick(Server &server, Client &client, const char *buffer)
{
    if (!buffer) {
        server.sendData(client.getClientFd(), getNumericReply(client, 431, client.getNickname()));
        return ;
    }
    for (size_t i = 0; buffer[i]; ++i) {
        if (!isAuthorized(buffer[i])) {
            server.sendData(client.getClientFd(), getNumericReply(client, 432, buffer));
            return ;
        }
    }
    for (vector<Client>::iterator it = server.getClientsList().begin(); it != server.getClientsList().end(); it++) {
        if (it->getNickname() == string(buffer)) {
            server.sendData(client.getClientFd(), getNumericReply(client, 433, buffer));
            return ;
            }
    }
    std::string nick = "NICK :";
    nick += buffer;
    server.sendData(client.getClientFd(), client.getHostname() + nick);
    client.setNickname(buffer);
}

/*
    @brief Store the username of the client
    @param server The server object
    @param client_fd The client file descriptor
    @param buffer The buffer containing the username

    If the client is already registered, the server will send a 462 error message.
    If the buffer is less than 4, the server will send a 461 error message.
    Otherwise, the server will send a 001 message.
*/
void user(Server& server, Client& client, const char *buffer)
{   
    vector<string> buff_split = split(string(buffer), ' ');
    if (buff_split.size() < 4) {
        server.sendData(client.getClientFd(), getNumericReply(client, 461, "USER"));
        return ;
    }
    if (client.getRegistrationStatus()) {
        server.sendData(client.getClientFd(), getNumericReply(client, 462, "USER")); 
        return ;
    }
    if (client.getUsername() == "")
        client.setUsername(string(buff_split[0]));
    server.sendData(client.getClientFd(), getNumericReply(client, 001, ""));
    client.setRegistrationStatus(true);
}