/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcamerly <lcamerly@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:43:11 by ocyn              #+#    #+#             */
/*   Updated: 2024/10/12 04:20:59 by lcamerly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

// Constructor
Client::Client(int fd , time_t t) : creationTime_(t),
nickname_(""), username_(""), realname_(""), password_(""),
 clientFd_(fd), ispassgiven_(false), isRegistered_(false)
{
	
}

// Destructor
Client::~Client()
{
}

// Overload
bool Client::operator==(const Client& rhs) const {
    return this->clientFd_ == rhs.clientFd_;
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
	this->joinedChannels_.push_back(&target);
}

/// @brief Leave a channel by removing it from the list of joined channels
/// @param target The channel to leave
void	Client::leaveChannel(Channel &target)
{
	std::vector<Channel *>::iterator it = std::find(this->joinedChannels_.begin(), this->joinedChannels_.end(), &target);

	if (it != this->joinedChannels_.end())
		this->joinedChannels_.erase(it);
}

/// @brief Find a channel in the list of joined channels
/// @param Name The name of the channel
/// @return Channel* The channel object if found, NULL otherwise
Channel	*Client::findChannel(string Name)
{
	for (std::vector<Channel*>::iterator i = this->joinedChannels_.begin(); i != this->joinedChannels_.end(); ++i)
	{
		if ((*i)->getName() == Name)
		{
			return (*i);
		}
	}
	return (NULL);
}

/*================== GETTERS ==================*/
string					Client::getNickname() const  			{ return this->nickname_; }
string					Client::getUsername() const  			{ return this->username_; }
string					Client::getRealname() const  			{ return this->realname_; }
string 					Client::getPassword() const				{ return this->password_; }
int						Client::getClientFd() const     		{ return this->clientFd_; }
bool					Client::getRegistrationStatus() const 	{ return this->isRegistered_; }
bool 					Client::getIspassgiven() const			{ return this->ispassgiven_; }
vector<Channel *>		Client::getJoinedChannels()				{ return this->joinedChannels_; }



/*================== SETTERS ==================*/
void					Client::setNickname(string nickname)	{ this->nickname_ = nickname; }
void					Client::setUsername(string username)   	{ this->username_ = username; }
void					Client::setIspassgiven(bool a)			{ this->ispassgiven_ = a; }
void					Client::setPassword(string password)	{ this->password_ = password; }
void					Client::setRealname(string realname)   	{ this->realname_ = realname; }
void					Client::setRegistrationStatus(bool status)	{ this->isRegistered_ = status; }
