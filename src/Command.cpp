/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocyn <ocyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 16:53:23 by ocyn              #+#    #+#             */
/*   Updated: 2024/09/23 17:31:03 by ocyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

// DEBUG function, temporary
void	ft_log(string content)
{
	std::cout << CYAN << content << RESET << std::endl;
}

void	mode(Server &server, Client &client, const string &buffer)
{
	string mode = getNumericReply(client, 324, buffer);
	// Sending MODE to client
	server.sendData(client.getClientFd(), mode);
}

void	who(Server &server, Client &client, const string &buffer)
{
	Channel	*channel = server.findChannel(buffer);
	if (!channel)
		return ;
	std::vector<Client *> List = channel->getMembers();
	std::cout << List.size() << std::endl;
	for (std::vector<Client *>::iterator it = List.begin(); it != List.end(); it++)
	{
		std::stringstream	arg;
		string				channelVisibility;

		// ft_log(channel->getName());
		channelVisibility = (channel->getVisibility());
		arg << buffer;
		arg << "_" << channelVisibility;
		arg << "_" << (*it)->getNickname();
		server.sendData(client.getClientFd(), getNumericReply(client, 353, arg.str()));
	}
	// Sending ENDOFNAME RPL to indicate the end of the list
	server.sendData(client.getClientFd(), getNumericReply(client, 366, buffer));
}

/*
	@brief Send JOIN command
	@param server The serverl object
	@param client The cient file descriptor
	@param channel The channel name
*/
void	join(Server &server, Client &client, const string &buffer)
{
	if (buffer[0] != '#')
	{
		std::cout << RED << "Channel name not valid" << std::endl;
		return ;
	}
	string join = "JOIN :";
	join += buffer;
	// Getting specified channel (or creating it if doesn't exist)

	Channel	&channel = server.findOrCreateChannel(buffer);
	ft_log("Channel joinned");
	client.joinChannel(channel);
	// Sending JOIN to client
	server.sendData(client.getClientFd(), client.getHostname() + join);
}

/*
	@brief Send a NICK command to the server
	@param server The server object
	@param client_fd The client file descriptor
	@param buffer The buffer containing the nickname
*/
void nick(Server &server, Client &client, const string &buffer)
{
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
	for (vector<Client>::iterator it = server.getClientsList().begin(); it != server.getClientsList().end(); it++) {
		if (it->getNickname() == string(buffer)) {
			client.setNickname("default" + intToString(client.getClientFd()));
			server.sendData(client.getClientFd(), getNumericReply(client, 433, buffer));
			return ;
			}
	}
	std::string nick = "NICK :";
	nick += buffer;
	server.sendData(client.getClientFd(), client.getHostname() + nick);
	client.setNickname(buffer);
}

/*
	@brief Store the username of the client
	@param server The server object
	@param client_fd The client file descriptor
	@param buffer The buffer containing the username

	If the client is already registered, the server will send a 462 error message.
	If the buffer is less than 4, the server will send a 461 error message.
	Otherwise, the server will send a 001 message.
*/
void user(Server& server, Client& client, const string &buffer)
{
	vector<string> buff_split = split(buffer, ' ');
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
	server.sendData(client.getClientFd(), getNumericReply(client, 001, ""));
	client.setRegistrationStatus(true);
}


/*
	Command: PRIVMSG
Parameters: <msgtarget> <text to be sent>

PRIVMSG is used to send private messages between users, as well as to
send messages to channels.  <msgtarget> is usually the nickname of
the recipient of the message, or a channel name.

The <msgtarget> parameter may also be a host mask (#<mask>) or server
mask ($<mask>).  In both cases the server will only send the PRIVMSG
to those who have a server or host matching the mask.  The mask MUST
have at least 1 (one) "." in it and no wildcards following the last
".".  This requirement exists to prevent people sending messages to
"#*" or "$*", which would broadcast to all users.  Wildcards are the
'*' and '?'  characters.  This extension to the PRIVMSG command is
only available to operators.

Numeric Replies:

		ERR_NORECIPIENT                 ERR_NOTEXTTOSEND
		ERR_CANNOTSENDTOCHAN            ERR_NOTOPLEVEL
		ERR_WILDTOPLEVEL                ERR_TOOMANYTARGETS
		ERR_NOSUCHNICK
		RPL_AWAY
*/

void privmsg(Server& server, Client& client, const string &buffer) {

	std::cout << buffer << std::endl;
	string target = string(buffer).substr(0, string(buffer).find(' '));
	if (string(buffer) == target) {
		server.sendData(client.getClientFd(), getNumericReply(client, 412, ""));
		return ;
	}
	string message = string(buffer).substr(string(buffer).find(' '));
	if (message.find("#*") != string::npos || message.find("$*") != string::npos) {
		server.sendData(client.getClientFd(), getNumericReply(client, 407, "PRIVMSG")); 
		return ;
	}
	for (vector<Client>::iterator it = server.getClientsList().begin(); it != server.getClientsList().end(); it++) {
		if (target == it->getNickname() && it->getRegistrationStatus() == true) {
		string msg; 
		msg += "PRIVMSG ";
		msg += buffer;
		server.sendData(client.getClientFd(), client.getHostname() + msg);
		}
	}
	// server.sendData(client.getClientFd(), getNumericReply(client, 401, target));
}
