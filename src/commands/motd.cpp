/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   motd.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcamerly <lcamerly@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 18:35:55 by jcuzin            #+#    #+#             */
/*   Updated: 2024/11/05 08:54:53 by lcamerly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Command.hpp"

void	motd(Server& server, Client& client)
{
	std::ifstream	file("motd.conf");
	if (!file.is_open())
		return (server.sendData(client.getClientFd(), getNumericReply(client, 422, "")));
	string	line;
	server.sendData(client.getClientFd(), getNumericReply(client, 375, ""));
	while (std::getline(file, line))
	{
		server.sendData(client.getClientFd(), getNumericReply(client, 372, line));
	}
	server.sendData(client.getClientFd(), getNumericReply(client, 376, ""));
}
