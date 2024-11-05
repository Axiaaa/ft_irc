#include "../Command.hpp"


///	@brief Send JOIN command
///	@param server The serverl object
/// @param client The cient file descriptor
/// @param channel The channel name
void	join(Server &server, Client &client, const string &buffer)
{
	if (client.getRegistrationStatus() != true) {
		server.sendData(client.getClientFd(), getNumericReply(client, 451, ""));
		return ;
	}
	vector<pair<string, string> > args = bufferParser(buffer);
	for (vector<pair<string, string> >::iterator it = args.begin(); it != args.end(); it++)
	{
		if ((*it).first[0] != '#' || (*it).first.substr(1).find('#') != string::npos)
		{
			server.sendData(client.getClientFd(), getNumericReply(client, 476, (*it).first));
			return ;
		}
		Channel &channel = server.findOrCreateChannel((*it).first, client);
		if (!(*it).second.empty())
			channel.setKey((*it).second), channel.addModString("+k");
		if (channel.getUserLimit() != 0 && channel.getMembers().size() >= (long unsigned int)channel.getUserLimit())
		{
			server.sendData(client.getClientFd(), getNumericReply(client, 471, channel.getName()));
			return;
		}
		if (!channel.getKey().empty())
		{
			if ((*it).second != channel.getKey())
			{
				server.sendData(client.getClientFd(), getNumericReply(client, 475, channel.getName()));
				return ;
			}
		}
		if (channel.isVisible() == PRIVATE && !channel.isInvited(client))
		{
			server.sendData(client.getClientFd(), getNumericReply(client, 473, channel.getName()));
			return ;
		}
		ft_log("Channel joined");
		client.joinChannel(channel);
		channel.addMember(client);
		server.sendData(client.getClientFd(), client.getHostname() + "JOIN " + channel.getName());
		channel.broadcastMessage(client.getHostname() + "JOIN " + channel.getName(), &client, &server);
		if (channel.getTopic() != "") {
			// Send the topic to the client if it exists
			server.sendData(client.getClientFd(), getNumericReply(client, 332, channel.getName() + "_" + channel.getTopic()));
			server.sendData(client.getClientFd(), getNumericReply(client, 333, channel.getName() + "_" + channel.getTopicSetBy() + "_" + channel.getTopicTime()));
		}
		// Remove the invitation if the client was invited
		if (channel.isInvited(client))
			channel.removeInvitation(client);
	}
}