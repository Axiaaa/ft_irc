/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcamerly <lcamerly@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 17:02:00 by ocyn              #+#    #+#             */
/*   Updated: 2024/10/07 10:57:08 by lcamerly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel():
name_("Default"),
topic_("")
{
}

Channel::Channel(string Name, string Topic):
name_(Name),
topic_(Topic)
{
	std::cout << MAGENTA << "Creating New Channel !\nName: " << Name << RESET << std::endl;
	// Verifier le nom du channel
	
}

Channel::Channel(string Name) :
    name_(Name),
    topic_("")
{
	std::cout << MAGENTA << "Creating New Channel !\nName: " << Name << RESET << std::endl;
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
int		Channel::getUserLimit()                 { return this->userLimit; }
string	Channel::getName()                      { return this->name_; }
string	Channel::getTopic()                     { return this->topic_; }
string  Channel::getKey()                       { return this->key_; }    
std::vector<Client *>& Channel::getMembers()    { return this->members_; }
string	Channel::getTopicTime()                 { return this->topicTime_; }
string	Channel::getTopicSetBy()                { return this->topicSetBy_; }
string	Channel::getCreationTime()              { return this->creationTime_; }
bool	Channel::isTopicOnlyOperator()          { return this->TopicOnlyOperator; }
string  Channel::getModString(Client& client)                  {
    string modstring = " ";
    for (std::vector<string>::iterator it = this->modstring.begin(); it != this->modstring.end(); it++)
    {
        modstring += *it;
    }
    if (this->isOperator(client))
        modstring += "+o";
    return modstring;
}
t_channelType Channel::isVisible()              { return this->visible_; }
bool Channel::isOpsListEmpty()                  { return true ? this->operators_.empty() : false; }
bool Channel::isInvited(Client &client)         { 
    for (vector<Client *>::iterator it = this->invited_.begin(); it != this->invited_.end(); it++)
    {
        if ((*it)->getClientFd() == client.getClientFd())
            return true;
    }
    return false;
}


// Setters 
void Channel::setTopic(string &topic)			{ topic_ = topic; }
void Channel::setKey(string key)				{ key_ = key; }
void Channel::setUserLimit(int limit)           { this->userLimit = limit; }
void Channel::setTopicOnlyOperator(bool status) { this->TopicOnlyOperator = status; }
void Channel::setTopicSetBy(string &topic)		{ topicSetBy_ = topic; }
void Channel::setTopicTime()                    { topicTime_ = intToString(time(0)); }
void Channel::setCreationTime()                 { creationTime_ = intToString(time(0)); }
void Channel::setVisible(t_channelType type)   { this->visible_ = type; }
void Channel::addModString(string mod)          { 
    
    for (vector<string>::iterator it = this->modstring.begin(); it != this->modstring.end(); it++)
    {
        if (*it == mod)
            return ;
    }
    this->modstring.push_back(mod);
}
void Channel::removeModString(string mod)       { 
    std::vector<string>::iterator it = std::find(modstring.begin(), modstring.end(), mod);
    if (it != modstring.end())
        modstring.erase(it);
}
void Channel::removeOperator(Client &client)    {
    std::vector<Client *>::iterator it = std::find(this->operators_.begin(), this->operators_.end(), &client);
    if (it != this->operators_.end())
        this->operators_.erase(it);
}
void Channel::addInvitation(Client &client)     {
    for (vector<Client *>::iterator it = this->invited_.begin(); it != this->invited_.end(); it++)
    {
        if ((*it)->getClientFd() == client.getClientFd())
            return ;
    }
    this->invited_.push_back(&client); 
}
void Channel::removeInvitation(Client &client)  {
    std::vector<Client *>::iterator it = std::find(this->invited_.begin(), this->invited_.end(), &client);
    if (it != this->invited_.end())
        this->invited_.erase(it);
}

