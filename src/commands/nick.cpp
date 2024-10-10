#include "../Command.hpp"


/*
	@brief Send a NICK command to the server
	@param server The server object
	@param client_fd The client file descriptor
	@param buffer The buffer containing the nickname
*/
void nick(Server &server, Client &client, const string &buffer)
{
	if (client.getRegistrationStatus() != true && client.getIspassgiven() == false) {
		if (client.getNickname().empty()) {
			client.setNickname(buffer);
			pass(server, client, "");
		}
		else {
			server.sendData(client.getClientFd(), getNumericReply(client, 451, ""));
			return ;
		}
		return ;
	}
	if (buffer.empty()) {
		server.sendData(client.getClientFd(), getNumericReply(client, 431, client.getNickname()));
		return ;
	}
	for (size_t i = 0; buffer[i]; ++i) {
		if (!isAuthorized(buffer[i])) {
			server.sendData(client.getClientFd(), getNumericReply(client, 432, buffer));
			return ;
		}
	}
	for (vector<Client *>::iterator it = server.getClientsList().begin(); it != server.getClientsList().end(); it++) {
		if ((*it)->getNickname() == string(buffer)) {
			server.sendData(client.getClientFd(), getNumericReply(client, 433, buffer));
			return ;
			}
	}
	std::string nick = "NICK :";
	nick += buffer;
	for (vector<Channel *>::iterator it = server.getChannelsList().begin(); it != server.getChannelsList().end(); it++) {
		if ((*it)->checkMember(client)) {
			string msg = client.getHostname();
			msg += "NICK :";
			msg += buffer;
			(*it)->broadcastMessage(msg, &client, &server);
		}
	}
	server.sendData(client.getClientFd(), client.getHostname() + nick);
	client.setNickname(buffer);
    if (client.getUsername() != "" && client.getRealname() != "" && client.getNickname() != "" && client.getRegistrationStatus() == false)
    {
        server.sendData(client.getClientFd(), getNumericReply(client, 001, ""));
        client.setRegistrationStatus(true);
    }
}
