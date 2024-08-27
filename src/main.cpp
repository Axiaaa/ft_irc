/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammirat <aammirat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:43:14 by ocyn              #+#    #+#             */
/*   Updated: 2024/08/17 17:41:34 by aammirat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	_receivingServ(Server &server);
void	_addFdClient(Server &server, int &max_sd);
int		_watchFds(Server &server, int &max_sd);
int		_newConnections(Server &server, char *password);
/*
int ft_strcmp(char *str, char *dest)
{
	int i;
	
	i = 0;
	while (str[i] && dest[i])
	{
		if (str[i] != dest[i])
			return (1);
		i++;
	}
	return (0);
}

int _IsPassWordCorrect(Server &server, int client_fd, char *password)
{
	if (FD_ISSET(client_fd, &server.getFdSet()))
	{
		char buffer[1024];
		int valread = recv(client_fd, buffer, 1024, 0);
		buffer[valread] = '\0';
		if (valread <= 0)
			return (1);
		else
		{
			char *tre = buffer;
			char pass[5] = "PASS";
			while (ft_strcmp(tre, pass) == 1)
				tre++;
			if (ft_strcmp(tre, password) != 0)
				return (1);
		}
	}
	return (0);
}*/

int main(int ac, char **av)
{
	if (ac < 3)
		return (1);

	// Initializating server
	Server server(av[1], av[2]);
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
		if (_newConnections(server, av[2]))
			break ;
		// Receiving clients datas
		_receivingServ(server);
	}
	return 0;
}

/*
Loop for each clients to check received messages
*/
void	_receivingServ(Server &server)
{
	std::vector<int> clients = server.getClientsList();
	for (std::vector<int>::iterator it = clients.begin(); it != clients.end(); )
	{
		int client_fd = *it;
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
					std::cout << "Client déconnecté, socket fd: " << client_fd << std::endl;
				else
					std::cerr << "Erreur lors de la réception des données du client, socket fd: " << client_fd << std::endl;
				close(client_fd);
				it = clients.erase(it);
			}
			else
			{
				// Affiche le message reçu du client
				std::cout << "Message reçu du client, socket fd: " << client_fd << std::endl;
				std::cout << "Contenu: [ " << buffer << " ]" << std::endl;
				std::cout << "Size: " << valread << std::endl;
				++it;
			}
		} 
		else
			++it;
	}
}

void	_addFdClient(Server &server, int &max_sd)
{
	vector<int>	clients = server.getClientsList();

	for (vector<int>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		int client_fd = *it;

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

int		_newConnections(Server &server, char *password)
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
			//if (_IsPassWordCorrect(server, newsockfd, password) != 0)
			//{
			//	std::cerr << "Erreur, mauvais mot de passe" << std::endl;
			//	return (1); 
			//}
			std::cout << "Nouvelle connexion acceptée, socket fd: " << newsockfd << std::endl;
			server.getClientsList().push_back(newsockfd);
		}
	return (0);
}