#include "../Command.hpp"

void    kick(Server& server, Client& client, const string &buffer)
{
    // Check if the client is registered
    if (client.getRegistrationStatus() != true) {
		server.sendData(client.getClientFd(), getNumericReply(client, 451, ""));
		return ;
	}
    // Check if the channel exists
    if (buffer.empty())
    {
        server.sendData(client.getClientFd(), getNumericReply(client, 461,""));
        return;
    }
    Channel *chan = server.findChannel(buffer.substr(0, buffer.find(' ')));
    if (!chan)
    {
        server.sendData(client.getClientFd(), getNumericReply(client, 403, buffer.substr(0, buffer.find(' '))));
        return ;
    }
    //Split the buffer into arguments
    vector<pair<string, string> > args = bufferParser(buffer.substr(buffer.find(' ') + 1, buffer.size()));
    //Set the reason for the kick (default is "The Kick-Hammer has spoken!")
    string reason = args[0].second;
    for (vector<pair<string, string> >::iterator it = args.begin(); it != args.end(); it++)
    {
        Client *target = server.findClient((*it).first);
        if (!target)
        {
            server.sendData(client.getClientFd(), getNumericReply(client, 401, (*it).first));
            return ;
        }
        if (!chan->checkMember(*target))
        {
            server.sendData(client.getClientFd(), getNumericReply(client, 441, chan->getName() + "_" + (*it).first));
            return ;
        }
        if (!chan->isOperator(client))
        {
            server.sendData(client.getClientFd(), getNumericReply(client, 482, chan->getName()));
            return ;
        }
        string kick = "KICK ";
        kick += chan->getName();
        kick += " ";
        kick += target->getNickname();
        if (reason == "")
            kick += " The kick-hammer has spoken!";
        else 
            kick += " " + reason;
        server.sendData(client.getClientFd(), client.getHostname() + kick);
        chan->broadcastMessage(client.getHostname() + kick, &client, &server);
        chan->removeMember(*target);
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
