#include "../Command.hpp"



void	mode(Server &server, Client &client, const string &buffer)
{
	if (client.getRegistrationStatus() != true) {
		server.sendData(client.getClientFd(), getNumericReply(client, 451, ""));
		return ;
	}
	string mode = getNumericReply(client, 324, buffer);
	// Sending MODE to client
	server.sendData(client.getClientFd(), mode);
}
