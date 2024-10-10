#include "../Command.hpp"

void part(Server& server, Client& client, const string &buffer)
{
    if (client.getRegistrationStatus() != true) {
        server.sendData(client.getClientFd(), getNumericReply(client, 451, ""));
        return ;
    }
    vector<pair<string, string> > args = bufferParser(buffer);
    for (vector<pair<string, string> >::iterator it = args.begin(); it != args.end(); it++)
    {
        Channel *chan = server.findChannel((*it).first);
        if (!chan)
        {
            server.sendData(client.getClientFd(), getNumericReply(client, 403, (*it).first));
            return ;
        }
        if (!chan->checkMember(client))
        {
            server.sendData(client.getClientFd(), getNumericReply(client, 442, chan->getName()));
            return ;
        }
        string part = "PART ";  
        part += (*it).first;
        if ((*it).second != "")
            part += " " + (*it).second;
        server.sendData(client.getClientFd(), client.getHostname() + part);
        chan->broadcastMessage(client.getHostname() + part, &client, &server);
        chan->removeMember(client);
        if (chan->getMembers().empty())
            server.removeChannel(chan);
        else if (server.findChannel(chan->getName()) && chan->isOpsListEmpty()) {
            std::stringstream ss;
            ss << "MODE " << chan->getName() << " +o " << chan->getMembers().front()->getNickname();
            for (std::vector<Client *>::iterator it = chan->getMembers().begin(); it != chan->getMembers().end(); it++)
                server.sendData((*it)->getClientFd(), client.getHostname() + ss.str());
            chan->addOperator(*chan->getMembers().front());
        }
    }
}