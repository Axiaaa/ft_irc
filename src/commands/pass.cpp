#include "../Command.hpp"


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