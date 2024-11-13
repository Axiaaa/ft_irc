#include "../Command.hpp"

void list(Server& server, Client& client, const string& buffer)
{
    if (client.getRegistrationStatus() != true)
    {
        server.sendData(client.getClientFd(), getNumericReply(client, 451, ""));
        return ;
    }
    (void)buffer;
    server.sendData(client.getClientFd(), getNumericReply(client, 321, ""));    
    for (vector<Channel *>::iterator it = server.getChannelsList().begin(); it != server.getChannelsList().end(); it++)
    {
        string usercount = intToString((*it)->getMembers().size());
        string topic = (*it)->getTopic() == "" ? "No topic" : (*it)->getTopic();
        server.sendData(client.getClientFd(), getNumericReply(client, 322, (*it)->getName() + "_" + usercount + "_" + topic));
    }
    server.sendData(client.getClientFd(), getNumericReply(client, 323, ""));
}