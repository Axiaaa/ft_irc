/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocyn <ocyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:43:11 by ocyn              #+#    #+#             */
/*   Updated: 2024/09/23 17:49:14 by ocyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
###########----BASIC MEMBER FUNCTIONS
*/

Server::Server(char* port, string password)
{
	(void)password;
	memset(&this->addr_, 0, sizeof(this->addr_));
	this->addr_.sin_port = htons(std::strtol(port, NULL, 10));
	this->addr_.sin_family = AF_INET;
	this->addr_.sin_addr.s_addr = INADDR_ANY;
	this->createSocket();
}

Server::~Server()
{
	for (vector<Client>::iterator it = this->clientsList_.begin(); it != this->clientsList_.end(); ++it)
	{
		close(it->getClientFd());
	}
	while (!this->channelsList_.empty())
		this->channelsList_.erase(this->channelsList_.begin());
	// Destruction des channels en attentes...
	close(this->socket_);
}


/*
###########----SPECIFICS MEMBER FUNCTIONS
*/

// Create and init Socket
void Server::createSocket()
{
	this->socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socket_ < 0)
		throw SocketCreationException();
	int optval = 1;
	if (setsockopt(this->socket_, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval)) != 0)
		close(this->socket_), throw SocketCreationException();
}

// Configure Socket
void Server::bindSocket()
{
	if (bind(this->socket_, (struct sockaddr*)&this->addr_, sizeof(this->addr_)) != 0)
		close(this->socket_), throw SocketBindException();
}

void Server::listenSocket()
{
	if (listen(this->socket_, 5) < 0)
		close(this->socket_), throw SocketListenException();
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
	

	// If the command is in the map, execute the corresponding function
	if (commands.find(command) != commands.end()) 
		commands[command](*this, client, arg.c_str());
	else if (command != "CAP" && command != "QUIT")
	{
		//If the command is not in the map, send an error message to the client
		std::string error = "ERROR :Unknown command ";
		error += command;
		error += "\r\n";
		this->sendData(client.getClientFd(), error);
	}
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
Channel	&Server::findOrCreateChannel(string Name)
{
	for (std::vector<Channel*>::iterator i = this->channelsList_.begin(); i != this->channelsList_.end(); ++i)
	{
		if ((*i)->getName() == Name)
		{
			// Channel found
			return (**i);
		}
	}
	// Channel not existing, creating new one
	Channel	*NewChannel = new Channel(Name);
	this->channelsList_.push_back(NewChannel);
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
	for (std::vector<Channel*>::iterator i = this->channelsList_.begin(); i != this->channelsList_.end(); ++i)
	{
		if ((*i)->getName() == Name)
		{
			return (*i);
		}
	}
	return (NULL);
}

// GETTERS 
int& 		Server::getSocket() 			{ return this->socket_; }
sockaddr 	Server::getAddr() 				{ return *(sockaddr*)&this->addr_; }
fd_set& 	Server::getFdSet() 				{ return this->fdSet_; }
vector<Client>& Server::getClientsList() 	{ return this->clientsList_; }
