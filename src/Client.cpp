/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcamerly <lcamerly@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:43:11 by ocyn              #+#    #+#             */
/*   Updated: 2024/08/20 18:32:16 by lcamerly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/*
###########----BASIC MEMBER FUNCTIONS
*/

Client::Client(int fd, sockaddr_in addr, time_t t) :
creationTime_(t), clientFd_(fd), isRegistered_(false), addr_(addr)
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
bool Client::getRegistrationStatus(){ return this->isRegistered_; }

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


void Client::init(string buffer, Server& server) { 
    vector<string> buff_split = split(buffer, ' ');
    if (buff_split.size() < 4)
        exit(42);
    this->username_ = buff_split[0];
    int seed = 0;
    for (std::vector<Client>::iterator it = server.getClientsList().begin(); it != server.getClientsList().end(); ++it) {
        seed++;
        if (*it == *this)
            this->id_ = rand() % seed;
    }
    this->realname_ = buff_split[3].substr(1);
    std::cout << "Client " << this->nickname_ << " connected" << std::endl;
    std::cout << "Client ID: " << this->id_ << std::endl;
    std::cout << "Client Realname: " << this->realname_ << std::endl;
}

string Client::getHostname() const {
    string prefix, name;
    if (this->getNickname().size() > 0) {
        prefix += this->getNickname() + "!" + this->getUsername() + "@localhost ";
        name = this->getNickname() + " ";
    }
    else {
        prefix += "localhost ";
        name = "* ";
    }
    return prefix + name;
}