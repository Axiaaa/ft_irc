/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcamerly <lcamerly@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:44:58 by ocyn              #+#    #+#             */
/*   Updated: 2024/09/23 12:00:21 by lcamerly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

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

#include "Client.hpp"
#include "Channel.hpp"
#include "Command.hpp"
#include "Exceptions.hpp"
#include "Utils.hpp"

using std::string;
using std::vector;
using std::map;
using std::set;

class Client;
class Channel;

class Server {

	private :
		sockaddr_in		addr_;
		vector<Client	*>		clientsList_;
		vector<Channel	*>		channelsList_;
		map<std::string, set<int> >channels;
		fd_set			fdSet_;
		int				socket_;
		string			password_;

	public :
		Server(char *port, string password);
		~Server();

		void		createSocket();
		void		bindSocket();
		void		listenSocket();
		void		startServer(char *port);
		void 		handleClientMessage(Client &client, string command, string arg);
		void		sendData(int client_fd, string data);

		Channel		&findOrCreateChannel(string Name, Client& client);

		int&			getSocket();

		sockaddr		getAddr();
		
		string			getPassword();
		
		vector<Client *>&	getClientsList();
		vector<Channel *>&	getChannelsList();
};


string getNumericReply(Client& client, int code, string arg);