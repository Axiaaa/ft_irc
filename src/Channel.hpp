/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcamerly <lcamerly@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 17:04:02 by ocyn              #+#    #+#             */
/*   Updated: 2024/09/24 01:23:10 by lcamerly         ###   ########.fr       */
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
		string					name_;
		string					topic_;
		string					topicSetBy_;
		string					topicTime_;
		std::vector<Client *>	members_;
        std::vector<Client *>	operators_;

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
        void    				addOperator(Client &client);
		void					broadcastMessage(const std::string& message, Client* sender, Server *server);	
        bool 					isOperator(Client &client);
		
		string					getName();
		string					getTopic();
		string					getTopicTime();
		string					getTopicSetBy();
		std::vector<Client *>&	getMembers();
};