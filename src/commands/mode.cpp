#include "../Command.hpp"

void	mode_k(int supr_or_add, Channel chan)
{

}
void	mode_o(int supr_or_add, Channel chan, Server &server, string buffer)
{

}
void	mode_l(int supr_or_add, Channel chan)
{

}
void	mode__t(int supr_or_add, Channel chan)
{

}
void	mode_i(int supr_or_add, Channel chan)
{

}

void	mode(Server &server, Client &client, const string &buffer)
{
	if (client.getRegistrationStatus() != true) {
		server.sendData(client.getClientFd(), getNumericReply(client, 451, ""));
		return ;
	}
	std::vector<std::string> buffsplit = split(buffer, ' ');
	string mode = getNumericReply(client, 324, buffer);
	if (buffsplit.size() >= 2)
	{	// Check if the command has enough arguments
		Channel chan = client.findChannel(buffsplit[0]);
		if (!chan)
		{
			server.sendData(client.getClientFd(), getNumericReply(client, 403, buffsplit[0]));
			return ;
		}
		int i = 1;
		int supr_or_add;
		while (buffsplit[i] && buffsplit[i] != NULL)
		{
			if (buffsplit[i][0] == '+') 
			{
				supr_or_add = 1;
			}
			else if (buffsplit[i][0] == '-')
			{
				supr_or_add = 0;
			}
			else
			{
				server.sendData(client.getClientFd(), getNumericReply(client, 421, "MODE"));
				return ;
			}
			if (buffsplit[i].size() != 2)
			{
				server.sendData(client.getClientFd(), getNumericReply(client, 421, "MODE"));
				return ;
			}
			switch (buffsplit[i][2])
			{
				case 'o' : mode_o(supr_or_add, chan, server, buffsplit[i + 1]) + i++;
				case 'k' : mode_k (supr_or_add, chan);
				case 'i' : mode_k (supr_or_add, chan);
				case 't' : mode_k (supr_or_add, chan);
				case 'l' : mode_k (supr_or_add, chan);
				default : server.sendData(client.getClientFd(), getNumericReply(client, 421, "MODE")) + return;
			}
			i++;
		}
		return ;
	}
	else 
	{
		server.sendData(client.getClientFd(), getNumericReply(client, 461, "MODE"));
        return ;
	}
	// Sending MODE to client
	server.sendData(client.getClientFd(), mode);
}
