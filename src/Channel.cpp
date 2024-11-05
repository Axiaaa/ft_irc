/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocyn <ocyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 17:02:00 by ocyn              #+#    #+#             */
/*   Updated: 2024/11/05 10:09:10 by ocyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

// Constructor
Channel::Channel():
_name("Default"),
_topic("")
{
}

// Constructor
Channel::Channel(string Name, string Topic):
_name(Name),
_topic(Topic)
{
	std::cout << MAGENTA << "Creating New Channel !\nName: " << Name << RESET << std::endl;	
}

// Constructor
Channel::Channel(string Name) :
    _name(Name),
    _topic("")
{
    _modstring.push_back("-k");
    _modstring.push_back("-l");
    _modstring.push_back("-t");
    _modstring.push_back("-i");
	std::cout << MAGENTA << "Creating New Channel !\nName: " << Name << RESET << std::endl;
}

// Destructor
Channel::~Channel()
{
    std::cout << RED << "Destroying Channel " << this->_name << RESET << std::endl;
    // this->_members.clear();
}

/// @brief Add a member to the channel
/// @param client The client to add
void Channel::addMember(Client &client)
{
    this->_members.push_back(&client);
}

/// @brief Remove a member from the channel (also remove from operators of the channel)
/// @param client The client to remove
void Channel::removeMember(Client &client)
{
    std::vector<Client *>::iterator it = std::find(this->_members.begin(), this->_members.end(), &client);
    if (it != this->_members.end())
        this->_members.erase(it);
    std::vector<Client *>::iterator it2 = std::find(this->_operators.begin(), this->_operators.end(), &client);
    if (it2 != this->_operators.end())
        this->removeOperator(client);
}

/// @brief Check if a client is a member of the channel
/// @param client The client to check
int Channel::checkMember(Client &client)
{    std::vector<Client *>::iterator it = std::find(this->_members.begin(), this->_members.end(), &client);
    if (it != this->_members.end())
        return 1;
    return 0;
}

/// @brief Add an operator to the channel
/// @param client The client to add
void Channel::addOperator(Client &client)
{
    this->_operators.push_back(&client);
}

/// @brief Broadcast a message to all clients in the channel
/// @param message The message to broadcast
/// @param sender The client who sent the message
/// @param server The server object
void Channel::broadcastMessage(const std::string& message, Client* sender, Server *server)
{
    for (std::vector<Client *>::iterator it = this->_members.begin(); it != this->_members.end(); ++it)
    {
        if ((*it)->getClientFd() != sender->getClientFd())
            server->sendData((*it)->getClientFd(), message);
    }
}

/// @brief Check if a client is an operator of the channel
/// @param client The client to check
/// @return True if the client is an operator, false otherwise
bool Channel::isOperator(Client &client)
{
    for (std::vector<Client *>::iterator it = this->_operators.begin(); it != this->_operators.end(); ++it)
    {
        if ((*it)->getClientFd() == client.getClientFd())
            return true;
    }
    return false;
}

/*================== GETTERS ==================*/
int		Channel::getUserLimit()                 { return this->_userLimit; }
bool	Channel::isTopicOnlyOperator()          { return this->_topicOnlyOperator; }
string	Channel::getName()                      { return this->_name; }
string	Channel::getTopic()                     { return this->_topic; }
string  Channel::getKey()                       { return this->_key; }    
string	Channel::getTopicTime()                 { return this->_topicTime; }
string	Channel::getTopicSetBy()                { return this->_topicSetBy; }
string	Channel::getCreationTime()              { return this->_creationTime; }
std::vector<Client *>& Channel::getMembers()    { return this->_members; }
t_channelType Channel::isVisible()              { return this->_visible; }
bool Channel::isOpsListEmpty()                  { return true ? this->_operators.empty() : false; }
bool Channel::isInvited(Client &client)         { 
    for (vector<Client *>::iterator it = this->_invited.begin(); it != this->_invited.end(); it++)
    {
        if ((*it)->getClientFd() == client.getClientFd())
            return true;
    }
    return false;
}
string  Channel::getModString(Client& client)                  {
    string _modstring = " ";
    for (std::vector<string>::iterator it = this->_modstring.begin(); it != this->_modstring.end(); it++)
    {
        _modstring += *it;
    }
    //Add operator status
    (this->isOperator(client)) ? _modstring += "+o" : _modstring += "-o"; 
    return _modstring;
}


/*================== SETTERS ==================*/
void Channel::setTopic(string &topic)			{ _topic = topic; }
void Channel::setKey(string key)				{ _key = key; }
void Channel::setUserLimit(int limit)           { this->_userLimit = limit; }
void Channel::setTopicOnlyOperator(bool status) { this->_topicOnlyOperator = status; }
void Channel::setTopicSetBy(string &topic)		{ _topicSetBy = topic; }
void Channel::setTopicTime()                    { _topicTime = intToString(time(0)); }
void Channel::setCreationTime()                 { _creationTime = intToString(time(0)); }
void Channel::setVisible(t_channelType type)   { this->_visible = type; }
void Channel::addModString(string mod)          { 
    
    for (vector<string>::iterator it = this->_modstring.begin(); it != this->_modstring.end(); it++)
    {
        if ((*it) == mod)
            return;
        if ((*it).substr(1) == mod.substr(1) && (*it)[0] != mod[0])
        {
            this->_modstring.erase(it);
            this->_modstring.push_back(mod);
            return;
        }
    }
    this->_modstring.push_back(mod);
}
void Channel::removeModString(string mod)       { 
    std::vector<string>::iterator it = std::find(_modstring.begin(), _modstring.end(), mod);
    if (it != _modstring.end())
        _modstring.erase(it);
}
void Channel::removeOperator(Client &client)    {
    std::vector<Client *>::iterator it = std::find(this->_operators.begin(), this->_operators.end(), &client);
    if (it != this->_operators.end())
        this->_operators.erase(it);
}
void Channel::addInvitation(Client &client)     {
    for (vector<Client *>::iterator it = this->_invited.begin(); it != this->_invited.end(); it++)
    {
        if ((*it)->getClientFd() == client.getClientFd())
            return ;
    }
    this->_invited.push_back(&client); 
}
void Channel::removeInvitation(Client &client)  {
    std::vector<Client *>::iterator it = std::find(this->_invited.begin(), this->_invited.end(), &client);
    if (it != this->_invited.end())
        this->_invited.erase(it);
}

