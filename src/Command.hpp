/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocyn <ocyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 21:17:59 by ocyn              #+#    #+#             */
/*   Updated: 2024/09/04 18:34:36 by ocyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"

class Server;
class Client;

void	nick(Server& server, Client& client, const string &buffer);
void	user(Server& server, Client& client, const string &buffer);
void	privmsg(Server& server, Client& client, const string &buffer);
void	join(Server &server, Client &client, const string &buffer);
void	who(Server &server, Client &client, const string &buffer);
void	mode(Server &server, Client &client, const string &buffer);

void	ft_log(string content);
