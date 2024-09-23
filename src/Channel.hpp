/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcamerly <lcamerly@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 17:04:02 by ocyn              #+#    #+#             */
/*   Updated: 2024/09/23 11:55:47 by lcamerly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"

using std::string;
using std::vector;

class	Client;
class   Server;

class Channel {

	private:
		string					_name;
		string					_topic;
		std::vector<Client *>	_members;
        std::vector<Client *>	_operators;

	public:
		Channel();
		Channel(string Name);
		Channel(string Name, string Topic);
		~Channel();
		
		string	getName();
		string	getTopic();
		
		std::vector<Client *>& getMembers();
		
		int		checkMember(Client &client);

		void	setName();
		void	setTopic();
		void	addMember(Client &client);
		void	removeMember(Client &client);
        void    addOperator(Client &client);
		void	broadcastMessage(const std::string& message, Client* sender, Server *server);

        bool isOperator(Client &client);
};