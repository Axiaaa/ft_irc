/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcuzin <jcuzin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:43:11 by ocyn              #+#    #+#             */
/*   Updated: 2024/10/31 18:38:42 by jcuzin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Command.hpp"

// Constructor
Server::Server(char* port, string pass)
{
	_password = pass;
	memset(&this->_addr, 0, sizeof(this->_addr));
	this->_addr.sin_port = htons(std::strtol(port, NULL, 10));
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_addr.s_addr = INADDR_ANY;
	this->createSocket();
}

// Destructor
Server::~Server()
{
    close(this->_socket);
    for (vector<Client *>::iterator i = this->_clientsList.begin(); i != this->_clientsList.end(); ++i)
    { 
		delete *i;
	}
	for (vector<Channel *>::iterator i = this->_channelsList.begin(); i != this->_channelsList.end(); ++i)
	{
		delete *i;
	}
    this->_clientsList.clear();
    this->_channelsList.clear();
}

/// @brief Create a socket
void Server::createSocket()
{
	this->_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_socket < 0)
		throw SocketCreationException();
	int optval = 1;
	if (setsockopt(this->_socket, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval)) != 0)
		close(this->_socket), throw SocketCreationException();
}

/// @brief Bind the socket to the address
void Server::bindSocket()
{
	if (bind(this->_socket, (struct sockaddr*)&this->_addr, sizeof(this->_addr)) != 0)
		close(this->_socket), throw SocketBindException();
}

/// @brief Listen on the socket
void Server::listenSocket()
{
	if (listen(this->_socket, 5) < 0)
		close(this->_socket), throw SocketListenException();
}

/// @brief Start the server
/// @param port The port to start the server on
void	Server::startServer(char *port)
{
	this->createSocket();
	this->bindSocket();
	this->listenSocket();
	std::cout << CYAN << "Serveur démarré sur le port " << port << RESET << std::endl;
}


/// @brief Handle a client message
/// @param client The client object
/// @param command The command
/// @param arg The argument of the command
void Server::handleClientMessage(Client &client, string command, string arg)
{
	// Create a map of commands and their corresponding functions to avoid a long list of if/else
	map<string, void(*)(Server&, Client&, const string &buffer)> commands;
	commands["NICK"] 	= 	nick;
	commands["USER"] 	= 	user;
	commands["PRIVMSG"] = 	privmsg;
	commands["JOIN"] 	= 	join;
	commands["MODE"] 	= 	mode;
	commands["WHO"] 	= 	who;
	commands["PASS"] 	= 	pass;
	commands["TOPIC"] 	= 	topic;
	commands["INVITE"] 	= 	invite;
	commands["KICK"] 	= 	kick;
	commands["PART"] 	= 	part;
	commands["PING"] 	= 	pong;
		

	// If the command is in the map, execute the corresponding function
	if (commands.find(command) != commands.end()) 
		commands[command](*this, client, arg.c_str());
	else if (command != "CAP" && command != "QUIT")
		this->sendData(client.getClientFd(), getNumericReply(client, 421, command));
}

/// @brief Send data to a client
/// @param client_fd The client file descriptor
/// @param data The data to send
void	Server::sendData(int client_fd, string data)
{
	std::cout << YELLOW << "Sending data to client " << client_fd << " : " << data << RESET << std::endl;
	data += "\r\n";
	if (send(client_fd, data.c_str(), data.size(), 0) == -1)
		throw SendFailedException();
}


/// @brief Find or create a channel
/// @param buffer  The buffer
/// @param client  The client object
/// @return Channel& The channel object
Channel	&Server::findOrCreateChannel(string buffer, Client& client)
{
	pair<string, string> ChannelData = splitFirstSpace(buffer);
	for (vector<Channel*>::iterator i = this->_channelsList.begin(); i != this->_channelsList.end(); ++i)
	{
		if ((*i)->getName() == ChannelData.first)
		{
			// Channel found
			return (**i);
		}
	}
	// Channel not existing, creating new one
	Channel	*NewChannel = new Channel(ChannelData.first);
	ft_log("aaaa" + ChannelData.second);
	NewChannel->addOperator(client);
	NewChannel->setCreationTime();
	NewChannel->setTopicOnlyOperator(false);
	NewChannel->setUserLimit(0);
	NewChannel->setVisible(PUBLIC);
	this->_channelsList.push_back(NewChannel);
	return (*NewChannel);
}

/// @brief Find a channel
/// @param Name The name of the channel
/// @return Channel* The channel object
Channel	*Server::findChannel(string Name)
{
	for (std::vector<Channel*>::iterator i = this->_channelsList.begin(); i != this->_channelsList.end(); ++i)
	{
		if ((*i)->getName() == Name)
		{
			return (*i);
		}
	}
	return (NULL);
}

/// @brief Find a client
/// @param Name The name of the client
/// @return Client* The client object
Client	*Server::findClient(string Name)
{
	for (std::vector<Client*>::iterator i = this->_clientsList.begin(); i != this->_clientsList.end(); ++i)
	{
		if ((*i)->getNickname() == Name)
		{
			return (*i);
		}
	}
	return (NULL);
}

/*================== GETTERS ==================*/

int&				Server::getSocket() 		{ return this->_socket; }
vector<Client *>& 	Server::getClientsList() 	{ return this->_clientsList; }
vector<Channel *>& 	Server::getChannelsList() 	{ return this->_channelsList; }
string				Server::getPassword()		{ return this->_password; }

/*================== SETTERS ==================*/
void 				Server::removeChannel(Channel *channel)
{
	std::vector<Channel *>::iterator it = std::find(this->_channelsList.begin(), this->_channelsList.end(), channel);
	if (it != this->_channelsList.end())
		this->_channelsList.erase(it), delete *it;
}
	