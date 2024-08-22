/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcamerly <lcamerly@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:43:11 by ocyn              #+#    #+#             */
/*   Updated: 2024/08/22 10:32:14 by lcamerly         ###   ########.fr       */
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

// Getters
string Client::getNickname() const  { return this->nickname_; }
string Client::getUsername() const  { return this->username_; }
string Client::getRealname() const  { return this->realname_; }
int long long Client::getId() const { return this->id_; }
int Client::getClientFd() const     { return this->clientFd_; }
bool Client::getRegistrationStatus() const { return this->isRegistered_; }

// Operator overloads
bool Client::operator==(const Client& rhs) const {
    return this->clientFd_ == rhs.clientFd_;
}

//Setters 

void Client::setNickname(string nickname)   { this->nickname_ = nickname; }
void Client::setUsername(string username)   { this->username_ = username; }
void Client::setRealname(string realname)   { this->realname_ = realname; }
void Client::setId(int long long id)        { this->id_ = id; }
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