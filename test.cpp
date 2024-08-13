#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <sys/wait.h>

using std::string;

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0) {
        std::cerr << "Erreur lors de la création du socket" << std::endl;
        return 1;
    }

    int optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(12345);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");


    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Erreur lors de la liaison du socket" << std::endl;
        close(sockfd), _exit(1);
    }

    if (listen(sockfd, 5) < 0) {
        std::cerr << "Erreur lors de la mise en écoute du socket" << std::endl;
        close(sockfd), _exit(1);
    }

    std::cout << "Serveur en écoute sur 127.0.0.1:12345" << std::endl;

    fd_set readfds;
    int max_sd = sockfd;
    std::vector<int> client_sockets;

    while (true) {
        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);

        for (int client_fd : client_sockets) {
            FD_SET(client_fd, &readfds);
            if (client_fd > max_sd)
                max_sd = client_fd;
        }

        int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if (activity < 0 && errno != EINTR) {
            std::cerr << "Erreur lors de l'utilisation de select()" << std::endl;
            break;
        }

        if (FD_ISSET(sockfd, &readfds)) {
            struct sockaddr_in client_addr;
            socklen_t client_len = sizeof(client_addr);
            int newsockfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_len);
            if (newsockfd < 0) {
                std::cerr << "Erreur lors de l'acceptation de la connexion" << std::endl;
                break;
            }

            std::cout << "Nouvelle connexion acceptée, socket fd: " << newsockfd << std::endl;
            client_sockets.push_back(newsockfd);
        }
        for (auto it = client_sockets.begin(); it != client_sockets.end(); ) {
            int client_fd = *it;

            if (FD_ISSET(client_fd, &readfds)) {
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
                    it = client_sockets.erase(it);
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

    for (int client_fd : client_sockets) {
        close(client_fd);
    }
    close(sockfd);

    return 0;
}
