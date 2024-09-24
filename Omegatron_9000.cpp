/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Omegatron_9000.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocyn <ocyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 20:08:30 by ocyn              #+#    #+#             */
/*   Updated: 2024/09/24 21:28:55 by ocyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// HEADERS

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <cerrno>
#include <algorithm>
#include <sys/wait.h>
#include <iterator>
#include <sstream>
#include <map>
#include <set>
#include <string>

using std::string;

// Colors codes
#define BLUE    "\033[1;34m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define CYAN    "\033[1;36m"
#define MAGENTA "\033[1;35m"
#define RESET   "\033[0m"

bool	isPortValid(const std::string &port);
void	ft_log(const string &content);
int		ft_stoi(const string &str);
int		errorlog(const string &content);
void	startingTitle();
void	shutdownBot();
int		sendMessage(int sock, const string &message);
int		connectToServer(const string ip, const string port, const string password);
string	getMessage(int sock);







// MAIN

int	main(int ac, char **av)
{
	if (ac < 2 || ac > 3)
		return (errorlog("Wrong usage !\nCorrect usage: ./Omegatron9000 <port> [password]"), 1);
	if (isPortValid(av[1]) == false)
		return (std::cerr << RED << "Invalid port\n" << RESET << std::endl, 1);
	string	port = "12345";
	string	password;
	if (ac == 3)
		password = av[2];
	startingTitle();
	int	sock = connectToServer(av[1], port, password);
	if (sock == -1)
		return (1);
	ft_log("Connected to server !");
	return (0);
}





void	shutdownBot()
{
	ft_log("\nShutting down Omegatron 9000...");
	exit(0);
}

int	sendMessage(int sock, const string &message)
{
	if (message.empty())
		return (1);
	if (send(sock, message.c_str(), message.size(), 0) == -1)
		return (errorlog("Send failed"));
	return (0);
}

string	getMessage(int sock)
{
	char	buffer[1024];
	int		valread;

	valread = recv(sock, buffer, 1024, 0);
	buffer[valread] = '\0';
	if (valread <= 0)
		return ("");
	return (buffer);
}

int	connectToServer(const string ip, const string port, const string password)
{
	int					sock;
	struct sockaddr_in	addr;
	std::map<string, string>	commands;

	commands["CAP"] = "CAP LS 302\r\n";
	commands["NICK"] = "NICK Omegatron9000\r\n";
	if (!password.empty())
		commands["PASS"] = "PASS" + password + "\r\n";
	commands["USER"] = "USER Omegatron9000 0 * :Omegatron9000\r\n";

	ft_log("Init Socket...");
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
		return (errorlog("Socket creation failed"));
	ft_log("Socket created !");
	addr.sin_family = AF_INET;
	addr.sin_port = htons(ft_stoi(port));
	addr.sin_addr.s_addr = inet_addr(ip.c_str());
	ft_log("Connecting to server...");
	// Not working for now
	if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		return (errorlog("Connection failed"));
	ft_log("Connected to server !");
	for (std::map<string, string>::iterator it = commands.begin(); it != commands.end(); ++it)
	{
		ft_log("\tSending command: " + it->first + " | " + it->second + "...");
		if (sendMessage(sock, it->second))
			return (1);
	}
	ft_log("Waiting for welcome message...");
	while (getMessage(sock).find("001 Omegatron9000:") == string::npos)
	{
		if (getMessage(sock).find("433") != string::npos)
			return (errorlog("Nickname already in use"));
	}
	ft_log("Welcome message received !");
	return (sock);
}

void	ft_log(const string &content)
{
	std::cout << CYAN << content << RESET << std::endl;
}

void	startingTitle()
{
	std::cout << "                                 ⣀⠠⠤⠐⠒⠒ ⠐⠒⠒ ⠤⢀⡀                  " << std::endl;
	std::cout << "                              ⢀⡠⠖⠉             ⠑⠢⡀               " << std::endl;
	std::cout << "                           ⢀⡴⠋  ⣰⣖⣉⣉⣉⣹⠆   ⢼⣭⣉⣉⣩⡷ ⠈⠱⡄             " << std::endl;
	std::cout << "                      ⢀ ⣀⠔⠲⡋    ⠙⠛⠉⠉⠉        ⠈⠉⠉⠁  ⠈⢢            " << std::endl;
	std::cout << "                     ⠰⠁⢨⡮⣤⡾⠤⢄           ⢃            ⢣           " << std::endl;
	std::cout << "                     ⡄ ⢹⢛⢿⣠⣇⣀⣱⣀⣤⣤⣶⣶⣿⣿⣶⣶⣤⣤⣀⣀⣤⣴⣶⣿⣿⣿⣿⣶⣶⣤⣾⣿          " << std::endl;
	std::cout << "                     ⡐⠉⠉⣾⣜⠉⣽⠉⢻⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠋⢹⡆         " << std::endl;
	std::cout << "                    ⢰⠁  ⡟⠁⠉⠏⡠⠊⠈⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇ ⠘⠃         " << std::endl;
	std::cout << "                   ⢀⣌ ⢀⡴⣀⡤⠤⠚⠁  ⠹⣿⣿⣿⣿⣿⣿⣿⣿⡿⠁⠹⣿⣿⣿⣿⣿⣿⣿⡿⠋  ⢀          " << std::endl;
	std::cout << "                   ⠈⠊⠥⠸⢿⠁    ⣀⣀ ⡈⠛⠿⠿⠿⠿⠟⠋   ⠈⠛⠻⠿⠿⣛⣫⡤⢦  ⢸    ⣀     " << std::endl;
	std::cout << "                       ⠈⡀    ⠋⠙⢻⣹⡛⠒⡖⠂⠠⡤⠤⢤⠤⠴⢶⠖⠲⣟⣿⡿⡋ ⠈⠁ ⡎   ⡘ ⢰    " << std::endl;
	std::cout << "                        ⢃       ⠘⢿⢖⡳  ⡇ ⠰⠖ ⢺⠒⠚⣏⡿⠊    ⢰⠁   ⡗⢁⣼    " << std::endl;
	std::cout << "                        ⠈⡄         ⠉⠲⠄⢇⣀⣀⣀⣀⡸⢤⠊⠁⣠⠄   ⢠⠃    ⡟⣤⠬⠦ ⡀ " << std::endl;
	std::cout << "                         ⠈⢆      ⠈⠲⣄        ⢀⡠⠞⠁   ⡰⠁    ⣴⠇⣣⣀⣀⣀⣈⡇" << std::endl;
	std::cout << "                           ⠑⣄      ⠈⠉⠉⠓⠒ ⠐⠒⠒⠋   ⢀⡠⠊      ⣿⣼⠁  ⠉⠙⡄" << std::endl;
	std::cout << "                             ⠑⠤⡀             ⢀⣠⠔⠋        ⠙⢷⠛⠒⠒⠲⡖⠁" << std::endl;
	std::cout << "                                ⠉⠒⠂⠠⠤⠤ ⠤⠤⠤⠐⠒⠊⠁            ⠘⠣⠤⠤⠴⠃ " << std::endl;
	std::cout << "\n\n";
	std::cout << " ██████╗ ███╗   ███╗███████╗ ██████╗  █████╗ ████████╗██████╗  ██████╗ ███╗   ██╗ " << std::endl;
	std::cout << "██╔═══██╗████╗ ████║██╔════╝██╔════╝ ██╔══██╗╚══██╔══╝██╔══██╗██╔═══██╗████╗  ██║ " << std::endl;
	std::cout << "██║   ██║██╔████╔██║█████╗  ██║  ███╗███████║   ██║   ██████╔╝██║   ██║██╔██╗ ██║ " << std::endl;
	std::cout << "██║   ██║██║╚██╔╝██║██╔══╝  ██║   ██║██╔══██║   ██║   ██╔══██╗██║   ██║██║╚██╗██║ " << std::endl;
	std::cout << "╚██████╔╝██║ ╚═╝ ██║███████╗╚██████╔╝██║  ██║   ██║   ██║  ██║╚██████╔╝██║ ╚████║ " << std::endl;
	std::cout << " ╚═════╝ ╚═╝     ╚═╝╚══════╝ ╚═════╝ ╚═╝  ╚═╝   ╚═╝   ╚═╝  ╚═╝ ╚═════╝ ╚═╝  ╚═══╝ " << std::endl;
	std::cout << "                                                                                  " << std::endl;
	std::cout << "                     █████╗  ██████╗  ██████╗  ██████╗                            " << std::endl;
	std::cout << "                    ██╔══██╗██╔═████╗██╔═████╗██╔═████╗                           " << std::endl;
	std::cout << "                    ╚██████║██║██╔██║██║██╔██║██║██╔██║                           " << std::endl;
	std::cout << "                     ╚═══██║████╔╝██║████╔╝██║████╔╝██║                           " << std::endl;
	std::cout << "                     █████╔╝╚██████╔╝╚██████╔╝╚██████╔╝                           " << std::endl;
	std::cout << "                     ╚════╝  ╚═════╝  ╚═════╝  ╚═════╝                            " << std::endl;
	std::cout << "\n\n###__Omegatron now online." << std::endl;
}

int	errorlog(const string &content)
{
	std::cerr << RED << "###__CRITICAL ERROR__###\n\t" << content << RESET << std::endl;
	return 1;
}

bool isPortValid(const std::string &port)
{
	if (port.empty())
		return false;
	for (std::string::const_iterator it = port.begin(); it != port.end(); ++it) {
		if (!std::isdigit(*it))
			return false;
	}
	if (port.size() > 1 && port[0] == '0')
		return false;
	if (ft_stoi(port) < 0 || ft_stoi(port) > 65535)
		return false;
	return true;
}

int	ft_stoi(const string &str)
{
	int	out;
	std::stringstream	ss(str);

	ss >> out;
	return out;
}
