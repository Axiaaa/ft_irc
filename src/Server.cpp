/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcamerly <lcamerly@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:43:11 by ocyn              #+#    #+#             */
/*   Updated: 2024/10/12 04:28:10 by lcamerly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Command.hpp"

// Constructor
Server::Server(char* port, string pass)
{
	password_ = pass;
	memset(&this->addr_, 0, sizeof(this->addr_));
	this->addr_.sin_port = htons(std::strtol(port, NULL, 10));
	this->addr_.sin_family = AF_INET;
	this->addr_.sin_addr.s_addr = INADDR_ANY;
	this->createSocket();
}

// Destructor
Server::~Server()
{
    close(this->socket_);
    for (std::vector<Client *>::iterator i = this->clientsList_.begin(); i != this->clientsList_.end(); ++i)
    {
        delete *i; 
    }
	for (std::vector<Channel *>::iterator i = this->channelsList_.begin(); i != this->channelsList_.end(); ++i)
	{
		delete *i;
	}
    this->clientsList_.clear();
    this->channelsList_.clear();
}

/// @brief Create a socket
void Server::createSocket()
{
	this->socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socket_ < 0)
		throw SocketCreationException();
	int optval = 1;
	if (setsockopt(this->socket_, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval)) != 0)
		close(this->socket_), throw SocketCreationException();
}

/// @brief Bind the socket to the address
void Server::bindSocket()
{
	if (bind(this->socket_, (struct sockaddr*)&this->addr_, sizeof(this->addr_)) != 0)
		close(this->socket_), throw SocketBindException();
}

/// @brief Listen on the socket
void Server::listenSocket()
{
	if (listen(this->socket_, 5) < 0)
		close(this->socket_), throw SocketListenException();
}

/// @brief Start the server
/// @param port 
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
	commands["NICK"] = nick;
	commands["USER"] = user;
	commands["PRIVMSG"] = privmsg;
	commands["JOIN"] = join;
	commands["MODE"] = mode;
	commands["WHO"] = who;
	commands["PASS"] = pass;
	commands["TOPIC"] = topic;
	commands["INVITE"] = invite;
	commands["KICK"] = kick;
	commands["PART"] = part;
	

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
	std::pair<string, string> ChannelData = splitFirstSpace(buffer);
	for (std::vector<Channel*>::iterator i = this->channelsList_.begin(); i != this->channelsList_.end(); ++i)
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
	this->channelsList_.push_back(NewChannel);
	return (*NewChannel);
}

/// @brief Find a channel
/// @param Name The name of the channel
/// @return Channel* The channel object
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

/// @brief Find a client
/// @param Name The name of the client
/// @return Client* The client object
Client	*Server::findClient(string Name)
{
	for (std::vector<Client*>::iterator i = this->clientsList_.begin(); i != this->clientsList_.end(); ++i)
	{
		if ((*i)->getNickname() == Name)
		{
			return (*i);
		}
	}
	return (NULL);
}

/*================== GETTERS ==================*/

int&				Server::getSocket() 		{ return this->socket_; }
vector<Client *>& 	Server::getClientsList() 	{ return this->clientsList_; }
vector<Channel *>& 	Server::getChannelsList() 	{ return this->channelsList_; }
string				Server::getPassword()		{ return this->password_; }

/*================== SETTERS ==================*/
void 				Server::removeChannel(Channel *channel)
{
	std::vector<Channel *>::iterator it = std::find(this->channelsList_.begin(), this->channelsList_.end(), channel);
	if (it != this->channelsList_.end())
		this->channelsList_.erase(it), delete *it;
}
	