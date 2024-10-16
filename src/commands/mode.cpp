#include "../Command.hpp"
#include "limits.h"

/// @brief Send 696 ERR_INVALIDMOREPARAM - More invalid parameters
/// @param client The client object
/// @param chan The channel object
/// @param server The server object
/// @param buffsplit The buffer split
/// @param err_message The error message
void send_ERR696(Client	&client, Channel &chan, Server &server, vector<string> buffsplit, string err_message);

/// @brief Handle the mode command for a channel (mode +i or -i)
/// @param server The server object
/// @param client The client object
/// @param buffer The buffer containing the command
void mode_i(t_type add_or_del, Channel &chan, Server &server, Client &client)
{
	if (add_or_del == ADD)
		chan.setVisible(PRIVATE);
	else if (add_or_del == DELETE)
		chan.setVisible(PUBLIC);
	std::stringstream ss;
	ss << "MODE " << chan.getName() << " " << (add_or_del == ADD ? "+i" : "-i");
	for (std::vector<Client *>::iterator it = chan.getMembers().begin(); it != chan.getMembers().end(); it++)
		server.sendData((*it)->getClientFd(), client.getHostname() + ss.str());
	chan.addModString((add_or_del == ADD ? "+i" : "-i"));	
}

/// @brief Handle the mode command for a channel (mode +t or -t)
/// @param add_or_del The type of mode (ADD or DELETE)
/// @param chan The channel object
/// @param server The server object
/// @param client The client object
void mode_topic(t_type add_or_del, Channel &chan, Server &server, Client &client)
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

/// @brief Handle the mode command for a channel (mode +o or -o)
/// @param add_or_del The type of mode (ADD or DELETE)
/// @param chan The channel object
/// @param server The server object
/// @param client The client object
void mode_o(t_type add_or_del, Channel &chan, Server &server, Client &client, pair<char, string>keys)
{
	Client *target;
	if (keys.second.empty()) { 
		send_ERR696(client, chan, server, vector<string>(1, string(1, keys.first)), ":Need more params");
		return ;
	}
	if (!(target = server.findClient(keys.second)))
	{
		server.sendData(client.getClientFd(), getNumericReply(client, 401, keys.second));
		return ;
	}
	if (chan.checkMember(*target) == 0)
	{
		server.sendData(client.getClientFd(), getNumericReply(client, 441, chan.getName() + "_" + target->getNickname()));
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

/// @brief	Handle the mode command (mode +l or -l)
/// @param add_or_del The type of mode (ADD or DELETE)
/// @param chan The channel object
/// @param server The server object
/// @param client The client object
/// @param keys	The mode arguments
void mode_l(t_type add_or_del, Channel &chan, Server &server, Client &client, pair<char, string>keys)
{
	unsigned long int limit = 0;
	if (!keys.second.empty())
		limit = atoi(keys.second.c_str());
	if (!limit || limit == ULLONG_MAX)
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

/// @brief Handle the mode command
/// @param add_or_del The type of mode (ADD or DELETE)
/// @param chan The channel object
/// @param server The server object
/// @param client The client object
/// @param keys The mode arguments
void mode_k(t_type add_or_del, Channel &chan, Server &server, Client &client,  pair<char, string>keys)
{
	if (keys.second.empty() && add_or_del == ADD) {
		server.sendData(client.getClientFd(), getNumericReply(client, 525, chan.getName()));
		return;
	}
	if (add_or_del == ADD)
		chan.setKey(string(keys.second));
	else if (add_or_del == DELETE)		
		chan.setKey("");
	std::stringstream ss;
	ss << "MODE " << chan.getName() << " " << (add_or_del == ADD ? "+k" : "-k") << " " << keys.second;
	for (std::vector<Client *>::iterator it = chan.getMembers().begin(); it != chan.getMembers().end(); it++)
		server.sendData((*it)->getClientFd(), client.getHostname() + ss.str());
	chan.addModString((add_or_del == ADD ? "+k" : "-k"));
}


/// @brief Split the mode arguments into a vector of pairs to bing a mode to its argument
/// @param mode_args The mode arguments
/// @return  The vector of pairs of mode arguments (Ex: <'o', "user1">)
vector<pair<char, string> > split_mode_args(string mode_args)
{	
	bool is_valid = true;
	if (mode_args[0] == '+' || mode_args[0] == '-') {
		if (mode_args[0] == '-')
			is_valid = false;
		mode_args = mode_args.substr(1);
	}
	vector<pair<char, string> > args;
    for (size_t i = 0; mode_args[i] && mode_args[i] != ' '; i++) 
    {
        pair<char, string> p(mode_args[i], "");
        args.push_back(p);
    
	}
	size_t pos = mode_args.find(' ');
	string remaining_args = "";
	if (pos != string::npos)
		remaining_args = mode_args.substr(pos + 1);
	vector<string> buffsplit = split(remaining_args, ' ');
	if (buffsplit.empty() || buffsplit.front() == mode_args)
		return args;
	size_t j = 0;
	for (size_t i = 0; i < args.size() && j < buffsplit.size(); i++)
	{	
		if (args[i].first == '-')
			is_valid = false;
		if (args[i].first == 'o' ||	args[i].first == 'l' ||	args[i].first == 'k')
			if (is_valid || args[i].first == 'o')
				args[i].second = buffsplit[j], j++;
		if (args[i].first == '+')
			is_valid = true;
	}
	return args;
}

void	mode(Server &server, Client &client, const string &buffer)
{
	if (client.getRegistrationStatus() != true) {
		server.sendData(client.getClientFd(), getNumericReply(client, 451, ""));
		return ;
	}
	std::vector<std::string> buffsplit = split(buffer, ' ');
	// Check if the client is in the channel and if the channel exists
	Channel *chan = server.findChannel(buffsplit.front());
	if (!chan)
	{
		server.sendData(client.getClientFd(), getNumericReply(client, 403, buffsplit.front()));
		return ;
	}
	if (!chan->checkMember(client))
	{
		server.sendData(client.getClientFd(), getNumericReply(client, 442, chan->getName()));
		return ;
	}
	if (buffsplit.size() < 2)
	{
		server.sendData(client.getClientFd(), getNumericReply(client, 324, chan->getName() + "_" + chan->getModString(client)));
		server.sendData(client.getClientFd(), getNumericReply(client, 329, chan->getName() + "_" + chan->getCreationTime()));
		return ;
	}
	vector<pair<char, string> > mode_args = split_mode_args(buffer.substr(buffer.find(' ') + 1));
	t_type add_or_del;
	// Check if the mode is to add or delete
	if (buffsplit[1][0] == '+')
		add_or_del = ADD;
	else if (buffsplit[1][0] == '-')
		add_or_del = DELETE;
	else
	{
		send_ERR696(client, *chan, server, buffsplit, ":Invalid ModString");
		return ;
	}
	// Check if the client is an operator
	if (!chan->isOperator(client))
	{
		server.sendData(client.getClientFd(), getNumericReply(client, 482, chan->getName()));
		return ;
	}
	for (vector<pair<char, string> >::iterator it = mode_args.begin(); it != mode_args.end(); it++)
	{
		// Loop through the mode arguments and call the appropriate function
		switch ((*it).first)
		{
			case 'o':
				mode_o(add_or_del, *chan, server, client, (*it));
				break;
			case 'l':
				mode_l(add_or_del, *chan, server, client, (*it));
				break;
			case 't':
				mode_topic(add_or_del, *chan, server, client);
				break;
			case 'i':
			 	mode_i(add_or_del, *chan, server, client);
			 	break;
			case 'k':
			 	mode_k(add_or_del, *chan, server, client, (*it));
				break;
			case '+':
				add_or_del = ADD;	
				break;
			case '-':
				add_or_del = DELETE;
				break;
			default:
				send_ERR696(client, *chan, server, buffsplit, ":Invalid ModString");
				return ;
		}
	}	 
}