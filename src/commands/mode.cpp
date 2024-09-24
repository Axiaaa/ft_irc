#include "../Command.hpp"



void	mode(Server &server, Client &client, const string &buffer)
{
	string mode = getNumericReply(client, 324, buffer);
	// Sending MODE to client
	server.sendData(client.getClientFd(), mode);
}
