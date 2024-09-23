/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocyn <ocyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 17:04:02 by ocyn              #+#    #+#             */
/*   Updated: 2024/09/23 17:22:17 by ocyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"

using std::string;
using std::vector;

class	Client;

class Channel {

	private:
		string					_name;
		string					_topic;
		std::vector<Client *>	_members;
		string					_visibility;

	public:
		Channel();
		Channel(string Name);
		Channel(string Name, string Topic);
		~Channel();
		string	getName();
		string	getTopic();
		string	getVisibility();
		std::vector<Client *> &getMembers();

		void	setName();
		void	setTopic();
		void	setVisibility(string visibility);
		void	addMember(Client &client);
		void	removeMember(Client &client);
		int		checkMember(Client &client);

		void	broadcastMessage(const std::string& message, Client* sender);
};
