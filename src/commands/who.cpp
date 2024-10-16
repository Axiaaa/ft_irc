#include "../Command.hpp"

void	who(Server &server, Client &client, const string &buffer)
{	
	// Checking if the client is registered
	if (client.getRegistrationStatus() != true) {
		server.sendData(client.getClientFd(), getNumericReply(client, 451, ""));
		return ;
	}
	// Getting specified channel (or creating it if doesn't exist)
	Channel	&channel = server.findOrCreateChannel(buffer, client);

	// Sending RPL_NAMREPLY to every client in the channel (w/ @ for operators)
	for (std::vector<Client *>::iterator it = channel.getMembers().begin(); it != channel.getMembers().end(); it++)
	{
		if (channel.isOperator(*(*it)) == true)
			server.sendData(client.getClientFd(), getNumericReply(client, 353, channel.getName() + "_@" + (*it)->getNickname()));
		else
			server.sendData(client.getClientFd(), getNumericReply(client, 353, channel.getName() + "_" + (*it)->getNickname()));
	}
	
	// Sending RPL_ENDOFNAMES to end the RPL_NAMREPLY list
	server.sendData(client.getClientFd(), getNumericReply(client, 366, channel.getName()));
}