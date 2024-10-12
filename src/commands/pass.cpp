#include "../Command.hpp"

// @brief Store the password of the client
// @param server The server object
// @param client_fd The client file descriptor
// @param buffer The buffer containing the password
void pass(Server& server, Client& client, const string &buffer)
{
    if (client.getPassword() != "")
    {
        server.sendData(client.getClientFd(), getNumericReply(client, 462, "PASS"));
        return ;
    }
    client.setPassword(buffer);
    if (client.getPassword() == "")
    {
        server.sendData(client.getClientFd(), getNumericReply(client, 461, "PASS"));
        return ;
    }
    if (client.getPassword() != server.getPassword())
    {
        server.sendData(client.getClientFd(), getNumericReply(client, 464, "PASS"));
        return ;
    }
    client.setIspassgiven(true);
}