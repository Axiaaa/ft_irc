#include "Server.hpp"
#include "NumericReplies.cpp"

/*
    @brief Send a NICK command to the server
    @param server The server object
    @param client_fd The client file descriptor
    @param buffer The buffer containing the nickname
*/
void nick(Server &server, Client &client, const char *buffer)
{
    std::string nick = "NICK :";
    nick += buffer;
    client.setNickname(buffer);
    server.sendData(client.getClientFd(), client.getHostname() + nick);
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