/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocyn <ocyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:43:11 by ocyn              #+#    #+#             */
/*   Updated: 2024/10/03 20:34:14 by ocyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/*
###########----BASIC MEMBER FUNCTIONS
*/

Client::Client(int fd) : 
_nickname(""), _username(""), _realname(""), _password(""),
 _clientFd(fd), _ispassgiven(false)
{
	
}

Client::~Client()
{
	for (std::vector<Channel *>::iterator it = this->_joinedChannels.begin(); it != this->_joinedChannels.end(); ++it)
	{
		(*it)->removeMember(*this);
	}
	this->getJoinedChannels().clear();
}

/*
###########----SPECIFICS MEMBER FUNCTIONS
*/

// Operator overloads
bool Client::operator==(const Client& rhs) const
{
	return this->_clientFd == rhs._clientFd;
}

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

void	Client::joinChannel(Channel &target)
{
	this->_joinedChannels.push_back(&target);
}

void	Client::leaveChannel(Channel &target)
{
	std::vector<Channel *>::iterator it = std::find(this->_joinedChannels.begin(), this->_joinedChannels.end(), &target);

	if (it != this->_joinedChannels.end())
		this->_joinedChannels.erase(it);
}

// Getters
string					Client::getNickname() const  			{ return this->_nickname; }
string					Client::getUsername() const  			{ return this->_username; }
string					Client::getRealname() const  			{ return this->_realname; }
string 					Client::getPassword() const				{ return this->_password; }
vector<Channel *>		Client::getJoinedChannels()				{ return this->_joinedChannels; }
int						Client::getClientFd() const     		{ return this->_clientFd; }
bool					Client::getRegistrationStatus() const 	{ return this->_isRegistered; }
bool 					Client::getIspassgiven() const			{ return this->_ispassgiven; }


//Setters
void					Client::setNickname(string nickname)	{ this->_nickname = nickname; }
void					Client::setUsername(string username)   	{ this->_username = username; }
void					Client::setIspassgiven(bool a)			{ this->_ispassgiven = a; }
void					Client::setPassword(string password)	{ this->_password = password; }
void					Client::setRealname(string realname)   	{ this->_realname = realname; }
void					Client::setRegistrationStatus(bool status)	{ this->_isRegistered = status; }
