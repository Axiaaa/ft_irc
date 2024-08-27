/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcamerly <lcamerly@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:43:11 by ocyn              #+#    #+#             */
/*   Updated: 2024/08/27 15:11:18 by lcamerly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/*
###########----BASIC MEMBER FUNCTIONS
*/

Client::Client(int fd, sockaddr_in addr, time_t t) :
nickname_(""), username_(""), realname_(""),
creationTime_(t), clientFd_(fd), addr_(addr)
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

// Getters
string Client::getNickname() const  			{ return this->nickname_; }
string Client::getUsername() const  			{ return this->username_; }
string Client::getRealname() const  			{ return this->realname_; }
int Client::getClientFd() const     			{ return this->clientFd_; }
bool Client::getRegistrationStatus() const 		{ return this->isRegistered_; }

//Setters
void Client::setNickname(string nickname)		{ this->nickname_ = nickname; }
void Client::setUsername(string username)   	{ this->username_ = username; }
void Client::setRealname(string realname)   	{ this->realname_ = realname; }
void Client::setRegistrationStatus(bool status) { this->isRegistered_ = status; }


string Client::getHostname() const {
	std::string prefix = ":";
	std::string name;
	if (this->getNickname() != "" && this->getUsername() != ""){
		prefix += this->getNickname() + "!" + this->getUsername() + "@localhost ";
		name = this->getNickname() + " ";
	}
	else{
		name = "* ";
		prefix += "localhost ";
	}
    return prefix;
}