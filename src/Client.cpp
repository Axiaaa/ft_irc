/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammirat <aammirat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:43:11 by ocyn              #+#    #+#             */
/*   Updated: 2024/09/25 10:31:44 by aammirat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/*
###########----BASIC MEMBER FUNCTIONS
*/

Client::Client(int fd) : 
nickname_(""), username_(""), realname_(""), password_(""),
 clientFd_(fd), ispassgiven_(false)
{
	
}

Client::~Client()
{
}

/*
###########----SPECIFICS MEMBER FUNCTIONS
*/

// Operator overloads
bool Client::operator==(const Client& rhs) const {
    return this->clientFd_ == rhs.clientFd_;
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
	this->joinedChannels_.push_back(&target);
}

void	Client::leaveChannel(Channel &target)
{
	std::vector<Channel *>::iterator it = std::find(this->joinedChannels_.begin(), this->joinedChannels_.end(), &target);

	if (it != this->joinedChannels_.end())
		this->joinedChannels_.erase(it);
}

// Getters
string					Client::getNickname() const  			{ return this->nickname_; }
string					Client::getUsername() const  			{ return this->username_; }
string					Client::getRealname() const  			{ return this->realname_; }
string 					Client::getPassword() const				{ return this->password_; }
vector<Channel *>		Client::getJoinedChannels()				{ return this->joinedChannels_; }
int						Client::getClientFd() const     		{ return this->clientFd_; }
bool					Client::getRegistrationStatus() const 	{ return this->isRegistered_; }
bool 					Client::getIspassgiven() const			{ return this->ispassgiven_; }


//Setters
void					Client::setNickname(string nickname)	{ this->nickname_ = nickname; }
void					Client::setUsername(string username)   	{ this->username_ = username; }
void					Client::setIspassgiven(bool a)			{ this->ispassgiven_ = a; }
void					Client::setPassword(string password)	{ this->password_ = password; }
void					Client::setRealname(string realname)   	{ this->realname_ = realname; }
void					Client::setRegistrationStatus(bool status)	{ this->isRegistered_ = status; }
