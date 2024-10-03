/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocyn <ocyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 17:02:00 by ocyn              #+#    #+#             */
/*   Updated: 2024/10/03 19:20:50 by ocyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel():
_name("Default"),
_topic("")
{
}

Channel::Channel(string Name, string Topic):
_name(Name),
_topic(Topic)
{
	std::cout << MAGENTA << "Creating New Channel !\n Name: " << Name << RESET << std::endl;
	// Verifier le nom du channel
	
}

Channel::Channel(string Name):
_name(Name)
{
	std::cout << MAGENTA << "Creating New Channel !\n Name: " << Name << RESET << std::endl;
}

Channel::~Channel()
{
	this->_members.clear();
}

void Channel::addMember(Client &client)
{
	// Add a member to the channel
	this->_members.push_back(&client);
}
void Channel::removeMember(Client &client)
{
	// Remove a member from the channel
	std::vector<Client *>::iterator it = std::find(this->_members.begin(), this->_members.end(), &client);
	if (it != this->_members.end())
		this->_members.erase(it);
}

int Channel::checkMember(Client &client)
{
	// Check if the client is a member of the channel
	std::vector<Client *>::iterator it = std::find(this->_members.begin(), this->_members.end(), &client);
	if (it != this->_members.end())
		return 1;
	return 0;
}

void Channel::addOperator(Client &client)
{
	this->_operators.push_back(&client);
}

void Channel::broadcastMessage(const std::string& message, Client* sender, Server *server)
{
	(void)sender;
	//Loop over all FDs in the channel to send the message to all clients
	for (std::vector<Client *>::iterator it = this->_members.begin(); it != this->_members.end(); ++it)
	{
		if ((*it)->getClientFd() != sender->getClientFd())
		{
			server->sendData((*it)->getClientFd(), message);
		}
	}
}
bool Channel::isOperator(Client &client)
{
	//Check if the client is an operator of the channel
	for (std::vector<Client *>::iterator it = this->_operators.begin(); it != this->_operators.end(); ++it)
	{
		if ((*it)->getClientFd() == client.getClientFd())
			return true;
	}
	return false;
}

// Getters
string	Channel::getName()						{ return this->_name; }
string	Channel::getTopic()						{ return this->_topic; }    
std::vector<Client *>& Channel::getMembers()	{ return this->_members; }
string	Channel::getTopicTime()					{ return this->_topicTime; }
string	Channel::getTopicSetBy()				{ return this->_topicSetBy; }


// Setters 
void Channel::setTopic(string &topic)			{ _topic = topic; }
void Channel::setTopicSetBy(string &topic)		{ _topicSetBy = topic; }
void Channel::setTopicTime()					{
		_topicTime = intToString(time(0));
	}