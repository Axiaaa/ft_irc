/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcamerly <lcamerly@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:44:58 by ocyn              #+#    #+#             */
/*   Updated: 2024/11/05 08:32:49 by lcamerly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

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
#include <cstdio>
#include <fstream>

#include "Client.hpp"
#include "Channel.hpp"
#include "Command.hpp"
#include "Exceptions.hpp"
#include "Utils.hpp"

using std::string;
using std::vector;
using std::map;
using std::set;
using std::pair;

class Client;
class Channel;

class Server {

	private :
		int					_socket;
		string				_password;
		sockaddr_in			_addr;
		vector<Client*>		_clientsList;
		vector<Channel*>	_channelsList;

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
		Client				*findClient(string nickname);
		void				removeChannel(Channel *channel);
};


string 	getNumericReply(Client& client, int code, string arg);

#endif