/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocyn <ocyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:43:11 by ocyn              #+#    #+#             */
/*   Updated: 2024/10/03 20:35:37 by ocyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Command.hpp"

void	ft_log(string content) {
	std::cout << CYAN << content << RESET << std::endl;
}


/*
###########----BASIC MEMBER FUNCTIONS
*/

Server::Server(char* port, string pass)
{
	this->_password = pass;
	memset(&this->_addr, 0, sizeof(this->_addr));
	this->_addr.sin_port = htons(std::strtol(port, NULL, 10));
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_addr.s_addr = INADDR_ANY;
	this->createSocket();
}

Server::~Server()
{
	for (vector<Client *>::iterator it = this->_clientsList.begin(); \
	it != this->_clientsList.end(); ++it)
	{
		close((*it)->getClientFd());
		delete (*it);
	}
	this->_clientsList.clear();
	for (vector<Channel *>::iterator it = this->_channelsList.begin(); \
	it != this->_channelsList.end(); ++it) {
		delete (*it);
	}
	this->_channelsList.clear();
	if (this->_socket >= 0)
	{
		close(this->_socket);
		this->_socket = -1;
	}
}
/*
###########----SPECIFICS MEMBER FUNCTIONS
*/

// Create and init Socket
void Server::createSocket()
{
	this->_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_socket < 0)
		throw SocketCreationException();
	int optval = 1;
	if (setsockopt(this->_socket, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval)) != 0)
		close(this->_socket), throw SocketCreationException();
}

// Configure Socket
void Server::bindSocket()
{
	if (bind(this-> _socket, (struct sockaddr*)&this->_addr, sizeof(this->_addr)) != 0)
		close(this-> _socket), throw SocketBindException();
}

void Server::listenSocket()
{
	if (listen(this-> _socket, 5) < 0)
		close(this-> _socket), throw SocketListenException();
}

void	Server::startServer(char *port)
{
	this->createSocket();
	this->bindSocket();
	this->listenSocket();
	std::cout << CYAN << "Serveur démarré sur le port " << port << RESET << std::endl;
}


// Execute the specified commnand
void Server::handleClientMessage(Client &client, string command, string arg)
{
	// Create a map of commands and their corresponding functions to avoid a long list of if/else
	map<string, void(*)(Server&, Client&, const string &buffer)> commands;
	commands["NICK"] = nick;
	commands["USER"] = user;
	commands["PRIVMSG"] = privmsg;
	commands["JOIN"] = join;
	commands["MODE"] = mode;
	commands["WHO"] = who;
	commands["PASS"] = pass;
	commands["TOPIC"] = topic;
	

	// If the command is in the map, execute the corresponding function
	if (commands.find(command) != commands.end()) 
		commands[command](*this, client, arg.c_str());
	else if (command != "CAP" && command != "QUIT")
		this->sendData(client.getClientFd(), getNumericReply(client, 421, command));
}

// Send data to a client
void	Server::sendData(int client_fd, string data)
{
	std::cout << YELLOW << "Sending data to client " << client_fd << " : " << data << RESET << std::endl;
	data += "\r\n";
	if (send(client_fd, data.c_str(), data.size(), 0) == -1)
		throw SendFailedException();
}

/*
	@brief Search in existings channels list if 
	the specified channel has already been created 
	and attempts to join it. Otherwise the channel 
	will automatically by created
	@param	Name The name of the choosen channel
	@return	The reference of the channel specified
*/
Channel	&Server::findOrCreateChannel(string Name, Client& client)
{
	for (std::vector<Channel*>::iterator i = this->_channelsList.begin(); i != this->_channelsList.end(); ++i)
	{
		if ((*i)->getName() == Name)
		{
			// Channel found
			return (**i);
		}
	}
	// Channel not existing, creating new one
	Channel	*NewChannel = new Channel(Name);
	NewChannel->addOperator(client);
	this->_channelsList.push_back(NewChannel);
	return (*NewChannel);
}

/*
	@brief Search in existings channels list if 
	the specified channel has already been created 
	and attempts to join it.
	@param	Name The name of the choosen channel
	@return	The reference of the channel specified
*/
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

// GETTERS 
int&				Server::getSocket() 		{ return this-> _socket; }
vector<Client *>& 	Server::getClientsList() 	{ return this->_clientsList; }
vector<Channel *>& 	Server::getChannelsList() 	{ return this->_channelsList; }
string				Server::getPassword()		{ return this->_password; }

// Setters 
