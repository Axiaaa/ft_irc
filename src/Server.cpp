/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocyn <ocyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:43:11 by ocyn              #+#    #+#             */
/*   Updated: 2024/08/15 12:27:15 by ocyn             ###   ########.fr       */
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
	for (vector<int>::iterator it = this->clientsList_.begin(); it != this->clientsList_.end(); ++it) {
		close(*it);
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
	if (setsockopt(this->socket_, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval)) < 0)
		close(this->socket_), throw SocketCreationException();
}

// Configure Socket
void Server::bindSocket()
{
	if (bind(this->socket_, (struct sockaddr*)&this->addr_, sizeof(this->addr_)) < 0)
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
	std::cout << "Serveur démarré sur le port " << port << std::endl;
}

// GETTERS (for Server's attributes)
int& Server::getSocket() { return this->socket_; }
sockaddr Server::getAddr() { return *(sockaddr*)&this->addr_; }
fd_set& Server::getFdSet() { return this->fdSet_; }
vector<int>& Server::getClientsList() { return this->clientsList_; }