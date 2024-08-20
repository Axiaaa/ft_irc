/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcamerly <lcamerly@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:43:14 by ocyn              #+#    #+#             */
/*   Updated: 2024/08/20 17:57:11 by lcamerly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	_receivingServ(Server &server);
void	_addFdClient(Server &server, int &max_sd);
int		_watchFds(Server &server, int &max_sd);
int		_newConnections(Server &server);

int main(int ac, char **av)
{
	if (ac < 2)
		return (1);

	// Initializating server
	Server server(av[1], "password");
	// Starting server
	server.startServer(av[1]);

	fd_set &fdset = server.getFdSet();
	int max_sd = server.getSocket();
	while (true)
	{
		FD_ZERO(&fdset);
		FD_SET(server.getSocket(), &fdset);

		// Adding new Clients to list
		_addFdClient(server, max_sd);
		// Watching file descriptors
		if (_watchFds(server, max_sd))
			break;
		// New coming connections handling
		if (_newConnections(server))
			break ;
		_receivingServ(server);
	}
	return 0;
}

/*
Loop for each clients to check received messages
*/
void	_receivingServ(Server &server)
{
	std::vector<Client> clients = server.getClientsList();
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); )
	{
		int client_fd = it->getClientFd();
		// Check new incoming datas
		if (FD_ISSET(client_fd, &server.getFdSet()))
		{
			// Get datas in buffer
			char buffer[1024];
			int valread = recv(client_fd, buffer, 1024, 0);
			buffer[valread] = '\0';

			// If no data received, error and delete client from list
			if (valread <= 0)
			{
				if (valread == 0)
					std::cout << RED << "Client déconnecté, socket fd: " << client_fd << RESET << std::endl;
				else
					std::cerr << "Erreur lors de la réception des données du client, socket fd: " << client_fd << std::endl;
				close(client_fd);
				it = clients.erase(it);
			}
			else
			{
				// Affiche le message reçu du client
				std::cout << GREEN << "Message reçu du client " << client_fd << ":\n" << buffer << RESET <<std::endl;

				// Split the buffer into commands and execute them one by one
				string commands = buffer;
				while (commands.find("\r\n") != string::npos)
				{
					string command = commands.substr(0, commands.find("\r\n"));
					commands = commands.substr(commands.find("\r\n") + 2);
					string arg = command.substr(command.find(" ") + 1);
					command = command.substr(0, command.find(" "));
					server.handleClientMessage(*it, command, arg);
				}
				++it;
			}
		}
		else
			++it;
	}
}

void	_addFdClient(Server &server, int &max_sd)
{
	vector<Client>	clients = server.getClientsList();

	for (vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		int client_fd = it->getClientFd();

		FD_SET(client_fd, &server.getFdSet());
		if (client_fd > max_sd)
			max_sd = client_fd;
	}
}

int	_watchFds(Server &server, int &max_sd)
{
	int activity = select(max_sd + 1, &server.getFdSet(), NULL, NULL, NULL);

	if (activity < 0 && errno != EINTR) {
		std::cerr << "Erreur lors de l'utilisation de select()" << std::endl;
		return (1);
	}
	return (0);
}

int		_newConnections(Server &server)
{
	if (FD_ISSET(server.getSocket(), &server.getFdSet()))
	{
			struct sockaddr_in client_addr;
			socklen_t client_len = sizeof(client_addr);
			int newsockfd = accept(server.getSocket(), (struct sockaddr*)&client_addr, &client_len);
			if (newsockfd < 0) {
				std::cerr << "Erreur lors de l'acceptation de la connexion" << std::endl;
				return (1);
			}
			std::cout << MAGENTA << "Nouvelle connexion acceptée, socket fd: " << newsockfd << RESET << std::endl;
			Client newClient(newsockfd, client_addr, time(NULL));
			newClient.setRegistrationStatus(true);
			server.getClientsList().push_back(newClient);
	}
	return (0);
}