/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammirat <aammirat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 17:04:02 by ocyn              #+#    #+#             */
/*   Updated: 2024/10/01 15:25:07 by aammirat         ###   ########.fr       */
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
		int						RequestOnly;
		int						TopicOnyOperator;
		int						NeedPassword; //must change to definite a password
		int						HasLimitUser; //must change the amount of limit user

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
		void					setRequestOnly(int i);
		void					setTopicOnyOperator(int i);
		void					setNeedPassword(int i);
		void					setHasLimitUser(int i);
		
		string					getName();
		string					getTopic();
		string					getTopicTime();
		string					getTopicSetBy();
		int						getRequestOnly();
		std::vector<Client *>&	getMembers();
};