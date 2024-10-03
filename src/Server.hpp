/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocyn <ocyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:44:58 by ocyn              #+#    #+#             */
/*   Updated: 2024/10/03 19:14:02 by ocyn             ###   ########.fr       */
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
#include <stdio.h>

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
		sockaddr_in			_addr;
		vector<Client*>		_clientsList;
		vector<Channel*>	_channelsList;
		//fd_set				_fdSet;
		string				_password;
		int					_socket;

	public :
		Server(char *port, string password);
		~Server();

		void				createSocket();
		void				bindSocket();
		void				listenSocket();
		void				startServer(char *port);
		void 				handleClientMessage(Client &client, string command, string arg);
		void				sendData(int client_fd, string data);
		int&				getSocket();

		Channel				&findOrCreateChannel(string Name, Client& client);
		Channel				*findChannel(string Name);
		string				getPassword();
		vector<Client *>&	getClientsList();
		vector<Channel *>&	getChannelsList();
};


string 	getNumericReply(Client& client, int code, string arg);
void	ft_log(string content);