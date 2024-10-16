/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Omegatron_9000.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcamerly <lcamerly@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 20:08:30 by ocyn              #+#    #+#             */
/*   Updated: 2024/10/17 00:30:07 by lcamerly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Omegatron_9000.hpp"

// MAIN

int	main(int ac, char **av)
{
	if (ac < 3 || ac > 4)
		return (errorlog("Wrong usage !\nCorrect usage: ./Omegatron9000 <ip> <port> [password]"));
	if (isPortValid(av[2]) == false)
		return (errorlog("Invalid port !"));

	string	ip = av[1];
	string	port = av[2];
	string	password;
	if (ac == 4)
		password = av[3];

	startingTitle();
	ft_log(RESET, "__Server infos:\n\tIP: " + ip + "\n\tPort: " + port + "\n\tPassword: " + password);

	int	sock = connectToServer(ip, port, password);
	if (sock == -1 || sock == 1)
		return (errorlog("Connection failed"));
	manualRemoteControl(sock);
	GPTMode(sock);
	ft_log(MAGENTA, "\nf*ck this sh*t imma out !\n");
	shutdownBot(sock);
	return (0);
}

string	removesQuotes(const string &str)
{
	string	newStr = str;
	newStr.erase(std::remove(newStr.begin(), newStr.end(), '\"'), newStr.end());
	newStr.erase(std::remove(newStr.begin(), newStr.end(), '\''), newStr.end());
	return (newStr);
}


// Use the OpenAI API to generate a response when someone calls the bot in a channel
int	GPTMode(int sock)
{
	ft_log(MAGENTA, "\n##_GPTMode ON\n");

	string	commandPrefix = "OG9 ";
	string	privateMsg = "PRIVMSG Omegatron9000 :";
	ft_log(YELLOW, "\t__Checking API status");
	string	welcomeMessage = sendOpenAIRequest("Write a welcome message and introduce yourself.");
	if (welcomeMessage.empty())
		return (errorlog("OpenAI API not working"));
	ft_log(GREEN, "API is working !");
	while (42)
	{
		string	message = getMessage(sock);
		size_t	index = message.find(commandPrefix);
		size_t	index2 = message.find(privateMsg);
		if (index2 != message.npos)
		{
			string	user = message.substr(1, message.find("!") - 1);
			if (user.empty())
				return (errorlog("Command's username not found"));
			string	command = message.substr(index2 + privateMsg.size(), message.size() - (index2 + privateMsg.size() + 2));
			if (command.empty())
				return (errorlog("OG9 command not valid"));
			if (command.find("\r\n") != string::npos)
				command = command.substr(0, command.find("\r\n"));
			ft_log(YELLOW, "\tSending [" + command + "] to " + user + " in private message");
			string	APIresponse = sendOpenAIRequest(removesQuotes(command));
			if (APIresponse.empty())
				errorlog("Response could not be generated");
			else
			{
				string	answer = "PRIVMSG " + user + " :To" + user + ": " + APIresponse;
				ft_log(YELLOW, "\tSending AI response to server...");
				if (answer.size() > 500)
					answer = "PRIVMSG " + user + " :To" + user + ": " + "Sorry, but my response is too long to be sent.";
				sendMessage(sock, answer);
				ft_log(GREEN, "AI response sent !");
			}
		}
		if (index != message.npos) 
		{
			index += 5;
			ft_log(YELLOW, "\t__Waiting for call...");
			ft_log(GREEN, "AI command request recieved !");
			string	command = message.substr(index, message.size() - (index + 2));
			if (command.empty())
				return (errorlog("OG9 command not valid"));
			if (command.find("\r\n") != string::npos)
				command = command.substr(0, command.find("\r\n"));
			string	user = message.substr(1, message.find("!") - 1);
			if (user.empty())
				return (errorlog("Command's username not found"));
			size_t	channel_i = message.find("PRIVMSG #") + 8;
			string	channel = message.substr(channel_i, (message.find(" ", channel_i) - channel_i));
			ft_log(YELLOW, "\tSending [" + command + "] to " + user + " in channel [" + channel + "]");
			string	APIresponse = sendOpenAIRequest(removesQuotes(command));
			if (APIresponse.empty())
				errorlog("Response could not be generated");
			else
			{
				string	answer = "PRIVMSG " + channel + " :To" + user + ": " + APIresponse;
				ft_log(YELLOW, "\tSending AI response to server...");
				if (answer.size() > 500)
					answer = "PRIVMSG " + channel + " :To" + user + ": " + "Sorry, but my response is too long to be sent.";
				sendMessage(sock, answer);
				ft_log(GREEN, "AI response sent !");
			}
		}
	}
	ft_log(MAGENTA, "\n##_GPTMode OFF\n");
	return (EXIT_SUCCESS);
}

string	sendOpenAIRequest(const string &command_content)
{
	string	command = OPENAI_API_CALL;
	size_t	command_index = command.find("\"content\": \"   ");
	if (command_index == command.npos)
		return ("");
	command.insert(command_index + 15, command_content);
	//ft_log(YELLOW, "Sending [" + command + "]");
	FILE	*fp = popen(command.c_str(), "r");
	if (fp == NULL)
		return (errorlog("fd could not be not working WTF"), "");
	char	buffer[1024];
	string	out;
	try
	{
		while (fgets(buffer, 1024, fp) != NULL)
			out += buffer;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		pclose(fp);
		return (errorlog("Error while reading out fd"), "");
	}
	pclose(fp);
	if (out.empty())
		return (errorlog("Reponse message not existing"), "");
	string	content = out.substr(1, out.size() - 3);
	if (content.empty())
		return (errorlog("Reponse message content not existing"), "");
	ft_log(MAGENTA, "[" + content + "]");
	return (content);
}

// Set the BOT to manual mode, allowing user to send datas to the server
void	manualRemoteControl(int sock)
{
	ft_log(MAGENTA, "\n##_ManualRemoteControl ON\n");
	ft_log(RESET, ".INFOS_____________\n\t_ENTER: Send\n\t_CTRL+D: Exit\n");
	string	line;
	string	response;
	while (!std::cin.eof())
	{
		if	(std::cin.eof())
			break;
		std::cout << "   <Omegatron_9000\\> ";
		std::getline(std::cin, line);
		if	(std::cin.eof())
			break;
		std::cout << std::endl;
		if (!line.empty())
			sendMessage(sock, line);
		response = getMessage(sock);
		if (!response.empty())
			std::cout << "   <server\\> " << response;
	}
	ft_log(MAGENTA, "\n##_ManualRemoteControl OFF\n");
}


void	shutdownBot(int sock)
{
	ft_log(RED, "\nShutting down Omegatron_9000...");
	sendMessage(sock, "QUIT :Omegatron out !");
	close(sock);
	sock = -1;
	exit(0);
}

int	sendMessage(int sock, const string &message)
{
	if (message.empty())
		return (1);
	string	toSend = message + "\r\n";
	if (send(sock, toSend.c_str(), toSend.size(), 0) == -1)
		return (errorlog("Send failed"));
	return (0);
}

string	getMessage(int sock)
{
	char	buffer[BUFFERSIZE];
	int		valread;

	valread = recv(sock, buffer, 1024, 0);
	buffer[valread] = '\0';
	if (valread <= 0)
		return ("");
	return (buffer);
}

int	connectToServer(const string ip, const string port, const string password)
{
	int							sock;
	struct sockaddr_in			addr;
	std::map<string, string>	command;

	command["CAP"] = "CAP LS 302";
	command["NICK"] = "NICK " NAME;
	if (!password.empty())
		command["PASS"] = "PASS " + password + "";
	command["USER"] = "USER " NAME" 0 * :" NAME;

	ft_log(YELLOW, "_Init Socket...");
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
		return (errorlog("Socket creation failed"));
	ft_log(GREEN, "\tSocket created !");
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(ft_stoi(port));
	addr.sin_addr.s_addr = inet_addr(ip.c_str());
	ft_log(YELLOW, "_Connecting to server...");
	if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		return (errorlog("Connection failed"));
	ft_log(GREEN, "\tConnected to server !");
	ft_log(YELLOW, "_Authenticating...");
	for (std::map<string, string>::iterator it = command.begin(); it != command.end(); ++it)
	{
		ft_log(YELLOW, "\tSending " + it->first + " command |--> " + it->second);
		if (sendMessage(sock, it->second))
			return (1);
	}
	ft_log(YELLOW, "Waiting for welcome message...");
	while (42)
	{
		displayMessage(sock);
		if (getMessage(sock).find("001") != string::npos)
			break ;
		if (getMessage(sock).find("432") != string::npos)
			return (errorlog("Erroneous nickname"));
		if (getMessage(sock).find("433") != string::npos)
			return (errorlog("Nickname already in use"));
	}
	ft_log(GREEN, "Welcome message received !");
	return (sock);
}

void	displayMessage(int sock)
{
	string		message = getMessage(sock);
	if (message.empty())
		return ;
	std::cout << "Message received |--> [ " << CYAN << message << RESET << " ]\n" << std::endl;
}

void	ft_log(const string &color, const string &content)
{
	std::cout << color << content << RESET << std::endl;
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