/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocyn <ocyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 17:04:02 by ocyn              #+#    #+#             */
/*   Updated: 2024/10/03 19:06:45 by ocyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"

using std::string;
using std::vector;

class	Client;
class	Server;

class Channel {

	private:
		string					_name;
		string					_topic;
		string					_topicSetBy;
		string					_topicTime;
		std::vector<Client *>	_members;
		std::vector<Client *>	_operators;

	public:
		Channel();
		Channel(string Name);
		Channel(string Name, string Topic);
		~Channel();
		
		int						checkMember(Client &client);
		void					setTopicTime();
		void					setTopicSetBy(string &topic);
		void					setTopic(string &topic);
		void					addMember(Client &client);
		void					removeMember(Client &client);
		void					addOperator(Client &client);
		void					broadcastMessage(const std::string& message, Client* sender, Server *server);	
		bool 					isOperator(Client &client);
		
		string					getName();
		string					getTopic();
		string					getTopicTime();
		string					getTopicSetBy();
		std::vector<Client *>&	getMembers();
};