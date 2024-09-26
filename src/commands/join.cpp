#include "../Command.hpp"


/*
	@brief Send JOIN command
	@param server The serverl object
	@param client The cient file descriptor
	@param channel The channel name
*/
void	join(Server &server, Client &client, const string &buffer)
{
	if (client.getRegistrationStatus() != true) {
		server.sendData(client.getClientFd(), getNumericReply(client, 451, ""));
		return ;
	}
	if (buffer[0] != '#')
	{
		std::cout << RED << "Channel name not valid" << std::endl;
		return ;
	}
	string join = "JOIN :";
	join += buffer;
	// Getting specified channel (or creating it if doesn't exist)
	ft_log("JOIN command detected");
	Channel	&channel = server.findOrCreateChannel(buffer, client);
	ft_log("Channel joinned");
	client.joinChannel(channel);
	channel.addMember(client);
	// Sending JOIN to client
	server.sendData(client.getClientFd(), client.getHostname() + join);
	channel.broadcastMessage(client.getHostname() + join, &client, &server);
	if (channel.getTopic() != "") {
		server.sendData(client.getClientFd(), getNumericReply(client, 332, channel.getName() + "_" + channel.getTopic()));
		server.sendData(client.getClientFd(), getNumericReply(client, 333, channel.getName() + "_" + channel.getTopicSetBy() + "_" + channel.getTopicTime()));
	}
}