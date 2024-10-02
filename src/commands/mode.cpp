#include "../Command.hpp"

void	mode_k(int supr_or_add, Channel chan, string password)
{
	if (supr_or_add == 1)
	{
		chan.setNeedPassword(supr_or_add, password);
	}
	else
		chan.setNeedPassword(supr_or_add, NULL);
}
void	mode_o(int supr_or_add, Channel chan, Server &server, string buffer)
{
	Client *client = server.findClient(buffer);
	if (supr_or_add == 1)
	{
		if (chan.isOperator(*client))
			return ;
		chan.addOperator(*client);
	}
	else
	{
		if (chan.isOperator((*client)))
			chan.removeOperator(*client);
	}
}
void	mode_l(int supr_or_add, Channel chan, int amount)
{
	chan.setHasLimitUser(supr_or_add * amount);
}
void	mode__t(int supr_or_add, Channel chan)
{
	chan.setTopicOnyOperator(supr_or_add);
}
void	mode_i(int supr_or_add, Channel chan)
{
	chan.setRequestOnly(supr_or_add);
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
		Channel *chan = client.findChannel(buffsplit[0]);
		if (!chan)
		{
			server.sendData(client.getClientFd(), getNumericReply(client, 401, buffsplit[0]));
			return ;
		}
		int i = 1;
		int supr_or_add;
		while ((int)buffsplit.size() > i)
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
			if (buffsplit[i][1] == 'k')
				mode_k (supr_or_add, *chan, "dada");
			else if (buffsplit[i][1] == 'i')
				mode_i (supr_or_add, *chan);
			else if (buffsplit[i][1] == 't')
				mode__t (supr_or_add, *chan);
			else if (buffsplit[i][1] == 'l')
				mode_l (supr_or_add, *chan, 2);
			else if (buffsplit[i][1] == 'o')
				mode_o(supr_or_add, *chan, server, buffsplit[i + 1]);
			else 
			{
				server.sendData(client.getClientFd(), getNumericReply(client, 421, "MODE"));
				return ;
			}
			if (buffsplit[i][1] == 'o')
				i++;
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
