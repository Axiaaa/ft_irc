/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocyn <ocyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 17:02:00 by ocyn              #+#    #+#             */
/*   Updated: 2024/09/23 22:19:20 by ocyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel():
_name("Default"),
_topic("Default"),
_visibility("=")
{
}

Channel::Channel(string Name, string Topic):
_name(Name),
_topic(Topic),
_visibility("=")
{
	std::cout << MAGENTA << "Creating New Channel !\n Name: " << Name << RESET << std::endl;
}

Channel::Channel(string Name):
_name(Name),
_visibility("=")
{
	std::cout << MAGENTA << "Creating New Channel !\n Name: " << Name << RESET << std::endl;
}

Channel::~Channel()
{
	this->_members.clear();
}


void	Channel::addMember(Client &client)
{
	//std::cout << "Channel " << this->getName() << " add the member " << client.getNickname() << std::endl;
	this->_members.push_back(&client);
}

void	Channel::removeMember(Client &client)
{
	std::vector<Client *>::iterator it = std::find(this->_members.begin(), this->_members.end(), &client);
	if (it != this->_members.end())
		this->_members.erase(it);
}

// Getters
string	Channel::getName() { return this->_name; }

string	Channel::getTopic() { return this->_topic; }

std::vector<Client *> &Channel::getMembers() { return this->_members; }

string	Channel::getVisibility() { return (this->_visibility); }

// Setters

void	Channel::setVisibility(string visibility)
{
	this->_visibility = visibility;
}