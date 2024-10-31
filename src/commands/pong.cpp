/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pong.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcuzin <jcuzin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 18:35:55 by jcuzin            #+#    #+#             */
/*   Updated: 2024/10/31 18:38:09 by jcuzin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Command.hpp"

void	pong(Server& server, Client& client, const string &buffer)
{
	server.sendData(client.getClientFd(), client.getHostname() + "PONG " + buffer);
}
