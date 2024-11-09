/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcamerly <lcamerly@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:43:11 by ocyn              #+#    #+#             */
/*   Updated: 2024/11/09 17:36:52 by lcamerly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

// Constructor
Client::Client(int fd) :
_clientFd(fd), _ispassgiven(false), _isRegistered(false), _shoulQuit(false),
_nickname(""), _username("") ,_realname(""), _password("")
{
	
}

// Destructor
Client::~Client()
{	
	ft_log("Destroying Client");
	// for (std::vector<Channel *>::iterator it = this->_joinedChannels.begin(); it != this->_joinedChannels.end(); ++it)
	// {
	// 	(*it)->removeOperator(*this);
	// 	(*it)->removeMember(*this);
	// }
	this->getJoinedChannels().clear();
	std::cout << RED << "Destroying Client " << _nickname  << " FD : " << _clientFd << RESET << std::endl;
}

// Overload
bool Client::operator==(const Client& rhs) const {
    return this->_clientFd == rhs._clientFd;
}

/// @brief Get the hostname of the client
/// @return  The hostname (nickname!username@localhost)
string	Client::getHostname() const
{
	std::string prefix = ":";
	std::string name;

	if (this->getNickname() != "" && this->getUsername() != "")
	{
		prefix += this->getNickname() + "!" + this->getUsername() + "@localhost ";
		name = this->getNickname() + " ";
	}
	else
	{
		name = "* ";
		prefix += "localhost ";
	}
	return prefix;
}

/// @brief Join a channel by adding it to the list of joined channels
/// @param target The channel to join
void	Client::joinChannel(Channel &target)
{
	this->_joinedChannels.push_back(&target);
}

/// @brief Leave a channel by removing it from the list of joined channels
/// @param target The channel to leave
void	Client::leaveChannel(Channel &target)
{
	std::vector<Channel *>::iterator it = std::find(this->_joinedChannels.begin(), this->_joinedChannels.end(), &target);
	if (it != this->_joinedChannels.end())
		this->_joinedChannels.erase(it);
}

/// @brief Find a channel in the list of joined channels
/// @param Name The name of the channel
/// @return Channel* The channel object if found, NULL otherwise
Channel	*Client::findChannel(string Name)
{
	for (std::vector<Channel*>::iterator i = this->_joinedChannels.begin(); i != this->_joinedChannels.end(); ++i)
	{
		if ((*i)->getName() == Name)
		{
			return (*i);
		}
	}
	return (NULL);
}

/*================== GETTERS ==================*/
string					Client::getNickname() const  			{ return this->_nickname; }
string					Client::getUsername() const  			{ return this->_username; }
string					Client::getRealname() const  			{ return this->_realname; }
string 					Client::getPassword() const				{ return this->_password; }
int						Client::getClientFd() const     		{ return this->_clientFd; }
bool					Client::getRegistrationStatus() const 	{ return this->_isRegistered; }
bool 					Client::getIspassgiven() const			{ return this->_ispassgiven; }
vector<Channel *>		Client::getJoinedChannels()				{ return this->_joinedChannels; }
string					Client::getCommand() const				{ return this->_command; }
bool					Client::getQuit() const					{ return this->_shoulQuit; }



/*================== SETTERS ==================*/
void					Client::appendCommand(string command)		{ this->_command += command; }
void					Client::setNickname(string nickname)		{ this->_nickname = nickname; }
void					Client::setUsername(string username)   		{ this->_username = username; }
void					Client::setIspassgiven(bool a)				{ this->_ispassgiven = a; }
void					Client::setPassword(string password)		{ this->_password = password; }
void					Client::setRealname(string realname)   		{ this->_realname = realname; }
void					Client::setRegistrationStatus(bool status)	{ this->_isRegistered = status; }
void					Client::setCommand(string command)			{ this->_command = command; }
void					Client::clearCommand()						{ this->_command.clear(); }
void					Client::setQuit(bool a)						{ this->_shoulQuit = a; }