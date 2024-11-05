/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcamerly <lcamerly@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 17:04:02 by ocyn              #+#    #+#             */
/*   Updated: 2024/11/05 08:31:33 by lcamerly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

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
		int						_userLimit;
		bool					_topicOnlyOperator;
		string					_name;
		string					_topic;
		string					_topicSetBy;
		string					_topicTime;
		string					_creationTime;
		string					_key;
		vector<string>			_modstring;
		t_channelType			_visible;
		std::vector<Client *>	_members;
        std::vector<Client *>	_operators;
		std::vector<Client *>	_invited;

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
		
		int						getUserLimit();
        bool 					isOperator(Client &client);
		bool					isOpsListEmpty();
		bool					isTopicOnlyOperator();
		bool					isInvited(Client &client);
		void					addModString(string mod);
		string					getName();
		string					getKey();
		string					getTopic();
		string					getTopicTime();
		string					getCreationTime();
		string					getTopicSetBy();
		string					getModString(Client& client);
		t_channelType			isVisible();
		std::vector<Client *>&	getMembers();
};

#endif