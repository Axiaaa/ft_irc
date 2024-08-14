/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocyn <ocyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:43:14 by ocyn              #+#    #+#             */
/*   Updated: 2024/08/14 11:53:55 by ocyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	_initServ(int ac, char **av, Server &server)
{
	server.createSocket();
	server.bindSocket();
	server.listenSocket();
	std::cout << "Serveur démarré sur le port " << av[1] << std::endl;
}

int main(int ac, char **av) {

	if (ac < 2)
		return (1);
	Server server(av[1], "password");
	_initServ(ac, av, server);

	fd_set fdset;
	int max_sd = server.getSocket();
	while (true)
	{
		FD_ZERO(&fdset);
		FD_SET(server.getSocket(), &fdset);

		//dfbfklnbfklblbmdv J'ai pas appris les vecteurs moi bon sang jvnfjinvejnvjkd jkd
		for (int client_fd : server.getClientsList())
		{
			FD_SET(client_fd, &fdset);
			if (client_fd > max_sd)
				max_sd = client_fd;
		}

		int activity = select(max_sd + 1, &fdset, NULL, NULL, NULL);

		if (activity < 0 && errno != EINTR) {
			std::cerr << "Erreur lors de l'utilisation de select()" << std::endl;
			break;
		}

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
		for (auto it = server.getClientsList().begin(); it != server.getClientsList().end(); ) {
			int client_fd = *it;
		
			if (FD_ISSET(client_fd, &fdset)) {
				char buffer[1024];
				int valread = recv(client_fd, buffer, 1024, 0);
				buffer[valread] = '\0';
				if (valread <= 0) {
					if (valread == 0) {
						std::cout << "Client déconnecté, socket fd: " << client_fd << std::endl;
					} else {
						std::cerr << "Erreur lors de la réception des données du client, socket fd: " << client_fd << std::endl;
					}
					close(client_fd);
					it = server.getClientsList().erase(it);
				} else {
					std::cout << "Message reçu du client, socket fd: " << client_fd << std::endl;
					std::cout << "Contenu: " << buffer << std::endl;
					std::cout << "Size: " << valread << std::endl;
				}
			} else {
				++it;
			}
		}
	}

	return 0;
}