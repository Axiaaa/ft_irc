/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcamerly <lcamerly@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:43:11 by ocyn              #+#    #+#             */
/*   Updated: 2024/08/20 11:28:37 by lcamerly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Command.hpp"

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
	for (vector<Client>::iterator it = this->clientsList_.begin(); it != this->clientsList_.end(); ++it) {
		close(it->getClientFd());
		}
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

void Server::handleClientMessage(int client_fd, string command, string arg)
{
	//Create a map of commands and their corresponding functions to avoid a long list of if/else
	map<string, void(*)(Server&, int, const char*)> commands;
	commands["NICK"] = nick;
	commands["USER"] = user;

	//If the command is in the map, execute the corresponding function
	if (commands.find(command) != commands.end()) {
		std::cout << YELLOW << "Sending command: " << command << " Arg: " << arg << RESET << std::endl,
		commands[command](*this, client_fd, arg.c_str());
	}
	else
	{
		//If the command is not in the map, send an error message to the client
		std::string error = "ERROR : Unknown command ";
		error += command;
		error += "\r\n";
		this->sendData(client_fd, error);
	}
}

// Send data to a client
void	Server::sendData(int client_fd, string data)
{
	if (send(client_fd, data.c_str(), data.size(), 0) == -1)
		throw SendFailedException();
}

// GETTERS 
int& 		Server::getSocket() { return this->socket_; }
sockaddr 	Server::getAddr() 	{ return *(sockaddr*)&this->addr_; }
fd_set& 	Server::getFdSet() 	{ return this->fdSet_; }
vector<Client>& Server::getClientsList() { return this->clientsList_; }