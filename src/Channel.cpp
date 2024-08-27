/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocyn <ocyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 17:02:00 by ocyn              #+#    #+#             */
/*   Updated: 2024/08/27 17:31:15 by ocyn             ###   ########.fr       */
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
}

Channel::~Channel()
{
}
