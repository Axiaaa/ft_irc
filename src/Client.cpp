/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcamerly <lcamerly@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:43:11 by ocyn              #+#    #+#             */
/*   Updated: 2024/08/20 11:23:48 by lcamerly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/*
###########----BASIC MEMBER FUNCTIONS
*/

Client::Client(int fd, sockaddr_in addr, time_t t) :
clientFd_(fd),  creationTime_(t), addr_(addr)
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

//Setters 

void Client::setNickname(string nickname)   { this->nickname_ = nickname; }
void Client::setUsername(string username)   { this->username_ = username; }
void Client::setRealname(string realname)   { this->realname_ = realname; }
void Client::setId(int long long id)        { this->id_ = id; }
