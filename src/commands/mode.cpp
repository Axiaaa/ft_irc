#include "../Command.hpp"

void mode_topic (t_type add_or_del, Channel &chan, Server &server, Client &client)
{
	if (add_or_del == ADD)
		chan.setTopicOnlyOperator(true);
	else if (add_or_del == DELETE)
		chan.setTopicOnlyOperator(false);
	std::stringstream ss;
	ss << "MODE " << chan.getName() << " " << (add_or_del == ADD ? "+t" : "-t");
	for (std::vector<Client *>::iterator it = chan.getMembers().begin(); it != chan.getMembers().end(); it++)
		server.sendData((*it)->getClientFd(), client.getHostname() + ss.str());
	chan.addModString((add_or_del == ADD ? "+t" : "-t"));
}


void mode_o(t_type add_or_del, Channel &chan, Server &server, Client &client, const string &nickname)
{
	Client *target;
	if (nickname.empty() || !(target = server.findClient(nickname)))
	{
		server.sendData(client.getClientFd(), getNumericReply(client, 401, client.getNickname()));
		return ;
	}
	if (add_or_del == ADD)
	{
		if (chan.isOperator(*target))
			return ;
		chan.addOperator(*target);
	}
	else if (add_or_del == DELETE)
	{
		if (!chan.isOperator(*target))
			return ;
		chan.removeOperator(*target);	
	}
	std::stringstream ss;
	ss << "MODE " << chan.getName() << " " << (add_or_del == ADD ? "+o" : "-o") << " " << target->getNickname();
	for (std::vector<Client *>::iterator it = chan.getMembers().begin(); it != chan.getMembers().end(); it++)
		server.sendData((*it)->getClientFd(), client.getHostname() + ss.str());
	if (chan.isOpsListEmpty())
	{
		target = chan.getMembers().front();
		target == &client ? target = chan.getMembers().back() : target = chan.getMembers().front();
		chan.addOperator(*target);
		ss.str("");
		ss << "MODE " << chan.getName() << " +o " << target->getNickname();
		for (std::vector<Client *>::iterator it = chan.getMembers().begin(); it != chan.getMembers().end(); it++)
			server.sendData((*it)->getClientFd(), client.getHostname() + ss.str());
	}
}

void mode_l(t_type add_or_del, Channel &chan, vector<string> buffer, Server &server, Client &client)
{
	unsigned long int limit = 0;
	if (buffer.size() >= 3)
		limit = atoi(buffer[2].c_str());
	if (!limit)
	{
		if (add_or_del == DELETE)
			chan.setUserLimit(0) , limit = 0;
		else
			chan.setUserLimit(chan.getMembers().size()), limit = chan.getMembers().size();
	}
	else {
		if (limit < chan.getMembers().size())
			return ;
		if (add_or_del == ADD)
			chan.setUserLimit(limit);
		else if (add_or_del == DELETE)
			chan.setUserLimit(0);
	}
	std::stringstream ss;
	ss << "MODE " << chan.getName() << " " << (add_or_del == ADD ? "+l" : "-l") << " " << limit;
	for (std::vector<Client *>::iterator it = chan.getMembers().begin(); it != chan.getMembers().end(); it++)
		server.sendData((*it)->getClientFd(), client.getHostname() + ss.str());
	chan.addModString((add_or_del == ADD ? "+l" : "-l"));
}

void mode_k(t_type add_or_del, Channel &chan, vector<string> keys, Server &server, Client &client)
{
	string key;
	if (keys.size() >= 3)
		key = keys[2];
	if (add_or_del == ADD)
		chan.setKey(string(key));
	else if (add_or_del == DELETE)		
		chan.setKey("");
	std::stringstream ss;
	ss << "MODE " << chan.getName() << " " << (add_or_del == ADD ? "+k" : "-k") << " " << key;
	for (std::vector<Client *>::iterator it = chan.getMembers().begin(); it != chan.getMembers().end(); it++)
		server.sendData((*it)->getClientFd(), client.getHostname() + ss.str());
	chan.addModString((add_or_del == ADD ? "+k" : "-k"));
}

void	mode(Server &server, Client &client, const string &buffer)
{
	if (client.getRegistrationStatus() != true) {
		server.sendData(client.getClientFd(), getNumericReply(client, 451, ""));
		return ;
	}
	std::vector<std::string> buffsplit = split(buffer, ' ');
	Channel *chan = server.findChannel(buffsplit.front());
	if (!chan)
	{
		server.sendData(client.getClientFd(), getNumericReply(client, 403, buffsplit.front()));
		return ;
	}
	if (buffsplit.size() < 2)
	{
		server.sendData(client.getClientFd(), getNumericReply(client, 324, chan->getName() + "_" + chan->getModString(client)));
		server.sendData(client.getClientFd(), getNumericReply(client, 329, chan->getName() + "_" + chan->getCreationTime()));
		return ;
	}
	vector <char> char_split = charsplit(buffsplit[1]);
	t_type add_or_del;
	if (char_split[0] == '+')
		add_or_del = ADD;
	else if (char_split[0] == '-')
		add_or_del = DELETE;
	else
	{
		string s = "696 " + client.getNickname() + " " + chan->getName() + " " + buffsplit[1];
		for (int i = 2; i < (int)buffsplit.size(); i++)
			s += " " + buffsplit[i];
		s += " :Invalid modestring";
		server.sendData(client.getClientFd(), client.getHostname() + s);
		return ;
	}
	if (!chan->isOperator(client))
	{
		server.sendData(client.getClientFd(), getNumericReply(client, 482, chan->getName()));
		return ;
	}
	for (vector <char>::iterator it = char_split.begin() + 1; it != char_split.end(); it++)
	{
		switch (*it)
		{
			case 'o':
				mode_o(add_or_del, *chan, server, client, buffsplit[2]);	
				break;
			case 'l':
				if (buffsplit.size() >= 3 || add_or_del == DELETE) {
			 		mode_l(add_or_del, *chan, buffsplit, server, client);
			 		break;
				}
				else
				{
					string s = "696 " + client.getNickname() + " " + chan->getName() + " " + buffsplit[1];
					for (int i = 2; i < (int)buffsplit.size(); i++)
						s += " " + buffsplit[i];
					s += " :Need more arguments";
					server.sendData(client.getClientFd(), client.getHostname() + s);
					break;
				}
			case 't':
				mode_topic(add_or_del, *chan, server, client);
				break;
			// case 'i':
			// 	mode_i(add_or_del, *chan);
			// 	break;
			case 'k':
				if (buffsplit.size() >= 3 || add_or_del == DELETE) {
			 		mode_k(add_or_del, *chan, buffsplit, server, client);
			 		break;
				}
				else
				{
					string s = "696 " + client.getNickname() + " " + chan->getName() + " " + buffsplit[1];
					for (int i = 2; i < (int)buffsplit.size(); i++)
						s += " " + buffsplit[i];
					s += " :Need more arguments";
					server.sendData(client.getClientFd(), client.getHostname() + s);
					break;
				}
			default:
				string s = "696 " + client.getNickname() + " " + chan->getName() + " " + buffsplit[1];
				for (int i = 2; i < (int)buffsplit.size(); i++)
					s += " " + buffsplit[i];
				s += " :Invalid modestring";
				server.sendData(client.getClientFd(), client.getHostname() + s);
				return ;
		}
	}	 
}
	// vector<char> buffsplit = charsplit(buffer);
	// string mode = getNumericReply(client, 324, buffer);
	// if (buffsplit.size() >= 2)
	// {	// Check if the command has enough arguments
	// 	Channel *chan = client.findChannel(buffsplit[0]);
	// 	if (!chan)
	// 	{
	// 		server.sendData(client.getClientFd(), getNumericReply(client, 401, buffsplit[0]));
	// 		return ;
	// 	}
	// 	int i = 1;
	// 	int supr_or_add;
	// 	while ((int)buffsplit.size() > i)
	// 	{
	// 		if (buffsplit[i][0] == '+') 
	// 		{
	// 			supr_or_add = 1;
	// 		}
	// 		else if (buffsplit[i][0] == '-')
	// 		{
	// 			supr_or_add = 0;
	// 		}
	// 		else
	// 		{
	// 			server.sendData(client.getClientFd(), getNumericReply(client, 421, "MODE"));
	// 			return ;
	// 		}
	// 		if (buffsplit[i].size() != 2)
	// 		{
	// 			server.sendData(client.getClientFd(), getNumericReply(client, 421, "MODE"));
	// 			return ;
	// 		}
	// 		if (buffsplit[i][1] == 'k')
	// 			mode_k (supr_or_add, *chan, "dada");
	// 		else if (buffsplit[i][1] == 'i')
	// 			mode_i (supr_or_add, *chan);
	// 		else if (buffsplit[i][1] == 't')
	// 			mode__t (supr_or_add, *chan);
	// 		else if (buffsplit[i][1] == 'l')
	// 			mode_l (supr_or_add, *chan, 2);
	// 		else if (buffsplit[i][1] == 'o')
	// 			mode_o(supr_or_add, *chan, server, buffsplit[i + 1]);
	// 		else 
	// 		{
	// 			server.sendData(client.getClientFd(), getNumericReply(client, 421, "MODE"));
	// 			return ;
	// 		}
	// 		if (buffsplit[i][1] == 'o')
	// 			i++;
	// 		i++;
	// 	}
	// 	return ;
	// }
	// else 
	// {
	// 	server.sendData(client.getClientFd(), getNumericReply(client, 461, "MODE"));
    //     return ;
	// }
	// // Sending MODE to client
	// server.sendData(client.getClientFd(), mode);

