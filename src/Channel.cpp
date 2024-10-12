/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcamerly <lcamerly@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 17:02:00 by ocyn              #+#    #+#             */
/*   Updated: 2024/10/12 04:24:23 by lcamerly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

// Constructor
Channel::Channel():
name_("Default"),
topic_("")
{
}

// Constructor
Channel::Channel(string Name, string Topic):
name_(Name),
topic_(Topic)
{
	std::cout << MAGENTA << "Creating New Channel !\nName: " << Name << RESET << std::endl;	
}

// Constructor
Channel::Channel(string Name) :
    name_(Name),
    topic_("")
{
    modstring.push_back("-k");
    modstring.push_back("-l");
    modstring.push_back("-t");
    modstring.push_back("-i");
	std::cout << MAGENTA << "Creating New Channel !\nName: " << Name << RESET << std::endl;
}

// Destructor
Channel::~Channel()
{
    std::cout << RED << "Destroying Channel " << this->name_ << RESET << std::endl;
}

/// @brief Add a member to the channel
/// @param client The client to add
void Channel::addMember(Client &client)
{
    this->members_.push_back(&client);
}

/// @brief Remove a member from the channel (also remove from operators of the channel)
/// @param client The client to remove
void Channel::removeMember(Client &client)
{
    std::vector<Client *>::iterator it = std::find(this->members_.begin(), this->members_.end(), &client);
    if (it != this->members_.end())
        this->members_.erase(it);
    std::vector<Client *>::iterator it2 = std::find(this->operators_.begin(), this->operators_.end(), &client);
    if (it2 != this->operators_.end())
        this->removeOperator(client);
}

/// @brief Check if a client is a member of the channel
/// @param client The client to check
int Channel::checkMember(Client &client)
{
    std::vector<Client *>::iterator it = std::find(this->members_.begin(), this->members_.end(), &client);
    if (it != this->members_.end())
        return 1;
    return 0;
}

/// @brief Add an operator to the channel
/// @param client The client to add
void Channel::addOperator(Client &client)
{
    this->operators_.push_back(&client);
}

/// @brief Broadcast a message to all clients in the channel
/// @param message The message to broadcast
/// @param sender The client who sent the message
/// @param server The server object
void Channel::broadcastMessage(const std::string& message, Client* sender, Server *server)
{
    (void)sender;
    for (std::vector<Client *>::iterator it = this->members_.begin(); it != this->members_.end(); ++it)
    {
        if ((*it)->getClientFd() != sender->getClientFd())
            server->sendData((*it)->getClientFd(), message);
        {
        }
    }
}

/// @brief Check if a client is an operator of the channel
/// @param client The client to check
/// @return True if the client is an operator, false otherwise
bool Channel::isOperator(Client &client)
{
    for (std::vector<Client *>::iterator it = this->operators_.begin(); it != this->operators_.end(); ++it)
    {
        if ((*it)->getClientFd() == client.getClientFd())
            return true;
    }
    return false;
}

/*================== GETTERS ==================*/
int		Channel::getUserLimit()                 { return this->userLimit; }
bool	Channel::isTopicOnlyOperator()          { return this->TopicOnlyOperator; }
string	Channel::getName()                      { return this->name_; }
string	Channel::getTopic()                     { return this->topic_; }
string  Channel::getKey()                       { return this->key_; }    
string	Channel::getTopicTime()                 { return this->topicTime_; }
string	Channel::getTopicSetBy()                { return this->topicSetBy_; }
string	Channel::getCreationTime()              { return this->creationTime_; }
std::vector<Client *>& Channel::getMembers()    { return this->members_; }
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


/*================== SETTERS ==================*/
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
        if ((*it) == mod)
            return;
        if ((*it).substr(1) == mod.substr(1) && (*it)[0] != mod[0])
        {
            this->modstring.erase(it);
            this->modstring.push_back(mod);
            return;
        }
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

