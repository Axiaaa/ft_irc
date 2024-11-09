/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcamerly <lcamerly@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:43:14 by ocyn              #+#    #+#             */
/*   Updated: 2024/11/09 04:44:34 by lcamerly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	_receivingServ(Server &server, fd_set *fdset);
void	_addFdClient(Server &server, int &max_sd, fd_set *fdset);
int		_watchFds(Server &server, int &max_sd, fd_set *fdset);
int		_newConnections(Server &server, fd_set *fdset);

void	_stopServer(int sig)
{
	(void)sig;
	throw SigInt();
}


int main(int ac, char **av)
{
	// Arguments checking (port and password)
	if (ac < 3)
		return (std::cerr << "Usage: ./ircserv <port> [password]\n" << std::endl, 1);
	if (isPortValid(av[1]) == false)
		return (std::cerr << "Invalid port\n" << std::endl, 1);

	// Initializating server
	Server server(av[1], av[2]);
	// Starting server
	server.startServer(av[1]);
	
	fd_set fdset;
	int max_sd = server.getSocket();
	signal(SIGINT, _stopServer);
	try {
		while (42)
		{
			FD_ZERO(&fdset);
			FD_SET(server.getSocket(), &fdset);

			// Adding new Clients to list
			_addFdClient(server, max_sd, &fdset);
			// Watching file descriptors
			if (_watchFds(server, max_sd, &fdset))
				break;
			// New coming connections handling
			if (_newConnections(server, &fdset))
				break ;
			_receivingServ(server, &fdset);
		}
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}

void	_receivingServ(Server &server, fd_set *fdset)
{
	std::vector<Client *> &clients = server.getClientsList();
	for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); )
	{
		int client_fd = (*it)->getClientFd();
		// Check new incoming datas
		if (FD_ISSET(client_fd, fdset))
		{
			// Get datas in buffer
			char buffer[1024];
			int valread = recv(client_fd, buffer, 1024, 0);
			buffer[valread] = '\0';

			// If no data received, error and delete client from list
			if (valread <= 0)
			{
				if (valread == 0)
					std::cout << RED << "Client disconnected, socket fd: " << client_fd << RESET << std::endl;
				else
					std::cerr << "Error during socket creation, socket fd: " << client_fd << std::endl;
				close(client_fd);
				delete *it;
				it = clients.erase(it);
			}
			else
			{
				// Display received message
				std::cout << GREEN << "New message from client " << client_fd << ":\n" << (*it)->getCommand() + buffer << RESET <<std::endl;

				// Split the buffer into commands and execute them one by one
				(*it)->appendCommand(buffer);
				//Check if the buffers > 500 bytes
				if ((*it)->getCommand().size() > 500)
				{
					server.sendData(client_fd, getNumericReply(*(*it), 417, ""));
					(*it)->setCommand((*it)->getCommand().substr((*it)->getCommand().find("\r\n") + 2));
					return ;
				}
				if ((*it)->getCommand().find("\r\n") == string::npos)
					return ; 
				while ((*it)->getCommand().find("\r\n") != string::npos)
				{
					string command = (*it)->getCommand().substr(0, (*it)->getCommand().find("\r\n"));
					pair<string, string> split = splitFirstSpace(command);
					server.handleClientMessage(*(*it), split.first, split.second);
					(*it)->setCommand((*it)->getCommand().substr((*it)->getCommand().find("\r\n") + 2));
				}
				++it;
			}
		}
		else
			++it;
	}
}

void	_addFdClient(Server &server, int &max_sd, fd_set *fdset)
{
	vector<Client *> clients = server.getClientsList();

	for (vector<Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		int client_fd = (*it)->getClientFd();

		FD_SET(client_fd, fdset);
		if (client_fd > max_sd)
			max_sd = client_fd;
	}
}

int		_watchFds(Server &server, int &max_sd, fd_set *fdset)
{
	(void)server;
	int activity = select(max_sd + 1, fdset, NULL, NULL, NULL);

	if (activity < 0) {
		std::cerr << "Error during select()" << std::endl;
		return (1);
	}
	return (0);
}

int		_newConnections(Server &server, fd_set *fdset)
{
	if (FD_ISSET(server.getSocket(), fdset))
	{
			struct sockaddr_in client_addr;
			socklen_t client_len = sizeof(client_addr);
			int newsockfd = accept(server.getSocket(), (struct sockaddr*)&client_addr, &client_len);
			if (newsockfd < 0) {
				std::cerr << "Error while accepting a new connection" << std::endl;
				return (1);
			}
			std::cout << MAGENTA << "New connection accepted, socket fd: " << newsockfd << RESET << std::endl;
			Client *newClient = new Client(newsockfd);
			newClient->setRegistrationStatus(false);
			server.getClientsList().push_back(newClient);
	}
	return (0);
}


/*
PASS awe
NICK test
USER test 0 * :realname
*/