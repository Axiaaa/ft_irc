/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocyn <ocyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:43:14 by ocyn              #+#    #+#             */
/*   Updated: 2024/08/15 12:34:18 by ocyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
Loop for each clients to check received messages
*/
void	_receivingServ(Server &server, fd_set &fdset)
{
		std::vector<int> clients = server.getClientsList();
		for (std::vector<int>::iterator it = clients.begin(); it != clients.end(); )
		{
			int client_fd = *it;

			// Check new incoming datas
			if (FD_ISSET(client_fd, &fdset))
			{
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
					std::cout << "Contenu: " << buffer << std::endl;
					std::cout << "Size: " << valread << std::endl;
					++it;
				}
			} 
			else
				++it;
		}
}

int main(int ac, char **av)
{
	if (ac < 2)
		return (1);
	Server server(av[1], "password"); // Init server
	server.startServer(av[1]); // Start the server

	fd_set &fdset = server.getFdSet();
	int max_sd = server.getSocket();
	while (true)
	{
		FD_ZERO(&fdset);
		FD_SET(server.getSocket(), &fdset);
		vector<int> clients = server.getClientsList();

		// Adding clients file descriptors
		for (vector<int>::iterator it = clients.begin(); it != clients.end(); ++it)
		{
			int client_fd = *it;
			FD_SET(client_fd, &fdset);
			if (client_fd > max_sd)
				max_sd = client_fd;
		}

		// Watching file descriptors
		int activity = select(max_sd + 1, &fdset, NULL, NULL, NULL);
		if (activity < 0 && errno != EINTR) {
			std::cerr << "Erreur lors de l'utilisation de select()" << std::endl;
			break;
		}

		// New coming connections handling
		if (FD_ISSET(server.getSocket(), &fdset)) {
			struct sockaddr_in client_addr;
			socklen_t client_len = sizeof(client_addr);
			int newsockfd = accept(server.getSocket(), (struct sockaddr*)&client_addr, &client_len);
			if (newsockfd < 0) {
				std::cerr << "Erreur lors de l'acceptation de la connexion" << std::endl;
				break;
			}
			std::cout << "Nouvelle connexion acceptée, socket fd: " << newsockfd << std::endl;
			server.getClientsList().push_back(newsockfd);

		}
		_receivingServ(server, fdset);
	}
	return 0;
}
