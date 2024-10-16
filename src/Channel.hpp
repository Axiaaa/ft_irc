/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcamerly <lcamerly@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 17:04:02 by ocyn              #+#    #+#             */
/*   Updated: 2024/10/15 17:52:28 by lcamerly         ###   ########.fr       */
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
		string					_name;
		string					_topic;
		string					_topicSetBy;
		string					_topicTime;
		std::vector<Client *>	_members;
        std::vector<Client *>	_operators;
		std::vector<Client *>	_invited;
		vector<string>			_modstring;
		string					_creationTime;
		string					_key;
		t_channelType			_visible;
		bool					_topicOnlyOperator;
		int						_userLimit;

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
		void					removeModString(string mod);
		void    				removeOperator(Client &client);
        void    				addOperator(Client &client);
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
		int						getUserLimit();
};