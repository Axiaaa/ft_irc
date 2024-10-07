#include "../Command.hpp"

void invite(Server &server, Client &client, const string &buffer)
{
    if (client.getRegistrationStatus() != true)
    {
        server.sendData(client.getClientFd(), getNumericReply(client, 451, ""));
        return ;
    }
    std::vector<std::string> buffsplit = split(buffer, ' ');
    Client *target = server.findClient(buffsplit.front());
    if (!target)
    {
        server.sendData(client.getClientFd(), getNumericReply(client, 401, buffsplit.front()));
        return ;
    }
    Channel *chan = server.findChannel(buffsplit[1]);
    if (!chan)
    {
        server.sendData(client.getClientFd(), getNumericReply(client, 403, buffsplit[1]));
        return ;
    }
    if (buffsplit.size() < 2)
    {
        server.sendData(client.getClientFd(), getNumericReply(client, 461, "INVITE"));
        return ;
    }
    if (!chan->isOperator(client) && chan->isVisible() == PRIVATE)
    {
        server.sendData(client.getClientFd(), getNumericReply(client, 482, chan->getName()));
        return ;
    }
    for (vector<Client *>::iterator it = chan->getMembers().begin(); it != chan->getMembers().end(); it++)
    {
        if ((*it)->getNickname() == target->getNickname())
        {
            server.sendData(client.getClientFd(), getNumericReply(client, 443, target->getNickname() + "_" + chan->getName()));
            return ;
        }
    }
    chan->addInvitation(*target);
    std::stringstream ss;
    ss << "INVITE " << target->getNickname() << " " << chan->getName();
    server.sendData(target->getClientFd(), client.getHostname() + ss.str());
    server.sendData(client.getClientFd(), getNumericReply(client, 341, target->getNickname() + "_" + chan->getName()));
}