/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcamerly <lcamerly@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 17:04:02 by ocyn              #+#    #+#             */
/*   Updated: 2024/10/12 04:21:05 by lcamerly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"

using std::string;
using std::vector;

typedef enum {
    ADD,
    DELETE
}   t_type;

typedef enum {
    PUBLIC,
    PRIVATE
}   t_channelType;

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
		std::vector<Client *>	invited_;
		vector<string>			modstring;
		string					creationTime_;
		string					key_;
		t_channelType			visible_;
		bool					TopicOnlyOperator;
		int						NeedPassword;
		int						userLimit;

	public:
		Channel();
		Channel(string Name);
		Channel(string Name, string Topic);
		~Channel();
		
		int						checkMember(Client &client);
		void					setTopicTime();
		void					setUserLimit(int limit);
		void					setCreationTime();
		void					setTopicSetBy(string &topic);
		void					setTopic(string &topic);
		void					setKey(string key);
		void					setTopicOnlyOperator(bool status);
		void					addMember(Client &client);
		void					removeMember(Client &client);
        void    				addOperator(Client &client);
		void    				removeOperator(Client &client);
		void					addInvitation(Client &client);	
		void					removeInvitation(Client &client);
		void					broadcastMessage(const std::string& message, Client* sender, Server *server);
		void					setVisible(t_channelType visible);
		
        bool 					isOperator(Client &client);
		bool					isOpsListEmpty();
		bool					isTopicOnlyOperator();
		bool					isInvited(Client &client);
		string					getName();
		string					getKey();
		string					getTopic();
		string					getTopicTime();
		string					getCreationTime();
		string					getTopicSetBy();
		string					getModString(Client& client);
		std::vector<Client *>&	getMembers();
		t_channelType			isVisible();
		void					addModString(string mod);
		void					removeModString(string mod);
		int						getUserLimit();
};