/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammirat <aammirat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 17:02:00 by ocyn              #+#    #+#             */
/*   Updated: 2024/10/01 15:24:17 by aammirat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel():
name_("Default"),
topic_("")
{
    RequestOnly = 0;
}

Channel::Channel(string Name, string Topic):
name_(Name),
topic_(Topic)
{
	std::cout << MAGENTA << "Creating New Channel !\n Name: " << Name << RESET << std::endl;
	// Verifier le nom du channel
	
}

Channel::Channel(string Name):
name_(Name)
{
	std::cout << MAGENTA << "Creating New Channel !\n Name: " << Name << RESET << std::endl;
}

Channel::~Channel()
{
    
}

void Channel::addMember(Client &client)
{
    // Add a member to the channel
    this->members_.push_back(&client);
}
void Channel::removeMember(Client &client)
{
    // Remove a member from the channel
    std::vector<Client *>::iterator it = std::find(this->members_.begin(), this->members_.end(), &client);
    if (it != this->members_.end())
        this->members_.erase(it);
}

int Channel::checkMember(Client &client)
{
    // Check if the client is a member of the channel
    std::vector<Client *>::iterator it = std::find(this->members_.begin(), this->members_.end(), &client);
    if (it != this->members_.end())
        return 1;
    return 0;
}

void Channel::addOperator(Client &client)
{
    this->operators_.push_back(&client);
}

void Channel::broadcastMessage(const std::string& message, Client* sender, Server *server)
{
    (void)sender;
    //Loop over all FDs in the channel to send the message to all clients
    for (std::vector<Client *>::iterator it = this->members_.begin(); it != this->members_.end(); ++it)
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
    for (std::vector<Client *>::iterator it = this->operators_.begin(); it != this->operators_.end(); ++it)
    {
        if ((*it)->getClientFd() == client.getClientFd())
            return true;
    }
    return false;
}

// Getters
string	Channel::getName()                      { return this->name_; }
string	Channel::getTopic()                     { return this->topic_; }    
std::vector<Client *>& Channel::getMembers()    { return this->members_; }
string	Channel::getTopicTime()                 { return this->topicTime_; }
string	Channel::getTopicSetBy()                { return this->topicSetBy_; }
int     Channel::getRequestOnly()               { return this->RequestOnly; }


// Setters 
void Channel::setRequestOnly(int i)             { RequestOnly = i; }
void Channel::setNeedPassword(int i)            { NeedPassword = i; }
void Channel::setTopicOnyOperator(int i)        { TopicOnyOperator = i; }
void Channel::setHasLimitUser(int i)            { HasLimitUser = i; }
void Channel::setTopic(string &topic)			{ topic_ = topic; }
void Channel::setTopicSetBy(string &topic)		{ topicSetBy_ = topic; }
void Channel::setTopicTime()                    {
        topicTime_ = intToString(time(0));
    }