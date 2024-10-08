#include "../Command.hpp"

void    kick(Server& server, Client& client, const string &buffer)
{
    vector<string> buffsplit = split(buffer, ' ');
    if (buffsplit.size() < 2)
    {
        server.sendData(client.getClientFd(), getNumericReply(client, 461, "KICK"));
        return ;
    }
    Channel *chan = server.findChannel(buffsplit[0]);
    if (!chan)
    {
        server.sendData(client.getClientFd(), getNumericReply(client, 403, buffsplit[0]));
        return ;
    }
    if (chan->checkMember(client) == 0)
    {
        server.sendData(client.getClientFd(), getNumericReply(client, 442, chan->getName()));
        return ;
    }
    if (chan->isOperator(client) == false)
    {
        server.sendData(client.getClientFd(), getNumericReply(client, 482, chan->getName()));
        return ;
    } 
    // Generate me a reason finder (the reason will start by :)
    // And the command can be like KICK username username username :reason
    string reason = " ";
    size_t reason_pos = buffer.find(" :");
    if (reason_pos != string::npos)
    {
        reason += buffer.substr(reason_pos + 2);
        buffsplit.resize(reason_pos); // Remove the reason part from buffsplit
    }
    else
    {
        reason += "The kick-hammer has spoken";
    }
    for (int i = 1; i < (int)buffsplit.size(); i++)
    {
        Client *target = NULL;
        for (vector<Client *>::iterator it = chan->getMembers().begin(); it != chan->getMembers().end(); it++)
        {
            if ((*it)->getNickname() == buffsplit[i])
            {
                target = *it;
                break ;
            }
        }
        if (target == NULL)
        { 
            if (!(buffsplit[i][0] == ':'))
                server.sendData(client.getClientFd(), getNumericReply(client, 401, buffsplit[i]));
            return ;
        }
        if (chan->checkMember(*target) == 0)
        {
            server.sendData(client.getClientFd(), getNumericReply(client, 441, chan->getName()));
            return ;
        }
        chan->broadcastMessage(client.getHostname() + "KICK " + chan->getName() + " " + target->getNickname() + reason, &client, &server);
        server.sendData(client.getClientFd(), client.getHostname() + "KICK " + chan->getName() + " " + target->getNickname() + reason);
        chan->removeMember(*target);
        if (chan->getMembers().size() == 0)
            server.removeChannel(chan);

    }
}
