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
		server.sendData(client.getClientFd(), getNumericReply(client, 476, buffer));
		return ;
	}
	string join = "JOIN ";
	join += buffer;
	// Getting specified channel (or creating it if doesn't exist)
	ft_log("JOIN command detected");
	Channel &channel = server.findOrCreateChannel(buffer, client);
	if (channel.getUserLimit() != 0 && channel.getMembers().size() >= (long unsigned int)channel.getUserLimit())
	{
		server.sendData(client.getClientFd(), getNumericReply(client, 471, channel.getName()));
		return ;
	}
	if (!channel.getKey().empty())
	{
		string key = buffer.substr(buffer.find(" ") + 1);
		if (key != channel.getKey())
		{
			server.sendData(client.getClientFd(), getNumericReply(client, 475, channel.getName()));
			return ;
		}
	}
	ft_log("Channel joinned");
	if (channel.isVisible() == PRIVATE && !channel.isInvited(client))
	{
		server.sendData(client.getClientFd(), getNumericReply(client, 473, channel.getName()));
		return ;
	}
	client.joinChannel(channel);
	channel.addMember(client);
	// Sending JOIN to client
	server.sendData(client.getClientFd(), client.getHostname() + join);
	channel.broadcastMessage(client.getHostname() + join, &client, &server);
	if (channel.getTopic() != "") {
		server.sendData(client.getClientFd(), getNumericReply(client, 332, channel.getName() + "_" + channel.getTopic()));
		server.sendData(client.getClientFd(), getNumericReply(client, 333, channel.getName() + "_" + channel.getTopicSetBy() + "_" + channel.getTopicTime()));
	}
	if (channel.isInvited(client))
		channel.removeInvitation(client);
}