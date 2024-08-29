/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocyn <ocyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 21:17:59 by ocyn              #+#    #+#             */
/*   Updated: 2024/08/29 21:38:26 by ocyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"

class Server;
class Client;

void	nick(Server& server, Client& client, const char *buffer);
void	user(Server& server, Client& client, const char *buffer);
void	privmsg(Server& server, Client& client, const char *buffer);
void	join(Server &server, Client &client, const char *buffer);
