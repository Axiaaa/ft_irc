#include "../Command.hpp"


/*
	@brief Store the username of the client
	@param server The server object
	@param client_fd The client file descriptor
	@param buffer The buffer containing the username

	If the client is already registered, the server will send a 462 error message.
	If the buffer is less than 4, the server will send a 461 error message.
	Otherwise, the server will send a 001 welcoming message.
*/
void user(Server& server, Client& client, const string &buffer)
{
	if (client.getRegistrationStatus() != true && client.getIspassgiven() == false) {
		server.sendData(client.getClientFd(), getNumericReply(client, 451, ""));
		return ;
	}
	vector<string> buff_split = split(buffer, ' ');
	if (!client.getIspassgiven())
		return	;
	if (buff_split.size() < 4) {
		server.sendData(client.getClientFd(), getNumericReply(client, 461, "USER"));
		return ;
	}
	if (client.getRegistrationStatus()) {
		server.sendData(client.getClientFd(), getNumericReply(client, 462, "USER")); 
		return ;
	}
	if (client.getUsername() == "")
		client.setUsername(buff_split[0]);
	if (client.getRealname() == "")
		client.setRealname(buff_split[3].substr(1));
    if (client.getNickname() != "" && client.getUsername() != "" && client.getRealname() != "")
	{
		server.sendData(client.getClientFd(), getNumericReply(client, 001, ""));
		server.sendData(client.getClientFd(), getNumericReply(client, 002, ""));
		server.sendData(client.getClientFd(), getNumericReply(client, 003, ""));
		server.sendData(client.getClientFd(), getNumericReply(client, 004, ""));
		server.sendData(client.getClientFd(), getNumericReply(client, 005, ""));
		server.sendData(client.getClientFd(), getNumericReply(client, 251, intToString(server.getClientsList().size())));
		server.sendData(client.getClientFd(), getNumericReply(client, 252, ""));
		server.sendData(client.getClientFd(), getNumericReply(client, 255, intToString(server.getClientsList().size())));
		motd(server, client);
		client.setRegistrationStatus(true);
		}
}
