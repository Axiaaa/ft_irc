/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocyn <ocyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 17:02:00 by ocyn              #+#    #+#             */
/*   Updated: 2024/08/29 22:18:27 by ocyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel():
_name("Default"),
_topic("Default")
{
}

Channel::Channel(string Name, string Topic):
_name(Name),
_topic(Topic)
{
	std::cout << MAGENTA << "Creating New Channel !\n Name: " << Name << RESET << std::endl;
	// Verifier le nom du channel
	
}

Channel::Channel(string Name):
_name(Name),
_topic(NULL)
{
	std::cout << MAGENTA << "Creating New Channel !\n Name: " << Name << RESET << std::endl;
}

Channel::~Channel()
{
}

string	Channel::getName() { return this->_name; }
string	Channel::getTopic() { return this->_topic; }

std::vector<Client *> Channel::getMembers() { return this->_members; }
