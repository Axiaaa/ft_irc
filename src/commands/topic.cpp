#include "../Command.hpp"


void topic(Server& server, Client& client, const string &buffer)
{
	if (client.getRegistrationStatus() != true) 
		return ;
	std::string nickname = client.getNickname();
	// Check if the command has enough arguments, if not, send the topic of the channel
	if (split(buffer, ' ').size() < 2) {
		Channel *channel = server.findChannel(buffer);
		if (server.findChannel(buffer) == NULL)
		{	// Check if channel exists
			server.sendData(client.getClientFd(), getNumericReply(client, 403, buffer));
			return ;
		}
		if (channel->getTopic().empty())
		{
			server.sendData(client.getClientFd(), getNumericReply(client, 331, channel->getName()));
			return ;
		}
		// Send the topic to the client
		server.sendData(client.getClientFd(), getNumericReply(client, 332, channel->getName() + "_" + channel->getTopic()));
		server.sendData(client.getClientFd(), getNumericReply(client, 333, channel->getName() + "_" + channel->getTopicSetBy() + "_" + channel->getTopicTime()));
		return ;
	}
	std::pair<std::string, std::string> split_buffer = splitFirstSpace(buffer);
	if (server.findChannel(split_buffer.first) == NULL)
	{	// Check if channel exists
		server.sendData(client.getClientFd(), getNumericReply(client, 403, split_buffer.first));
		return ;
	}
	if (server.findChannel(split_buffer.first)->checkMember(client) == 0)
	{	// Check if client is in the channel
		server.sendData(client.getClientFd(), getNumericReply(client, 442, split_buffer.first));
		return ;
	}
	Channel *channel = server.findChannel(split_buffer.first);

	split_buffer.second = split_buffer.second.substr(1);
	if (channel->isTopicOnlyOperator() && !channel->isOperator(client))
	{ // Check if client is operator
		server.sendData(client.getClientFd(), getNumericReply(client, 482, channel->getName()));
		return ;
	}
	// Set values (topic, time, set by nickname)
	channel->setTopic(split_buffer.second);
	channel->setTopicTime();
	channel->setTopicSetBy(nickname);
	string topic = "TOPIC ";
	topic += split_buffer.first;
	topic += " :";
	topic += split_buffer.second;
	// Broadcast message to all clients in the channel
	for (std::vector<Client *>::iterator it = channel->getMembers().begin(); it != channel->getMembers().end(); it++)
		server.sendData((*it)->getClientFd(), client.getHostname() + topic);
}
