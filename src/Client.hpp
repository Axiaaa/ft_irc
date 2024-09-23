/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocyn <ocyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:44:58 by ocyn              #+#    #+#             */
/*   Updated: 2024/09/23 20:48:59 by ocyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"
#include "time.h"

using std::string;
using std::vector;

class Channel;

class Client {

	private :
		string				nickname_;
		string				username_;
		string				realname_;
		time_t				creationTime_;
		string				password_;
		int					clientFd_;
		bool				isRegistered_;
		sockaddr_in			addr_;
		std::vector<Channel *>	channels_;
		
	public :
		Client(int fd, sockaddr_in addr, time_t t);
		~Client();
		string					getNickname() const;
		string					getUsername() const;
		string					getRealname() const;
		string					getHostname() const;
		std::vector<Channel *>	getJoinedChannels();

		bool					getRegistrationStatus() const;
		void					setRegistrationStatus(bool status);
		void					setNickname(string nickname);
		void					setUsername(string username);
		void					setRealname(string realname);
		int						getClientFd() const;

		void					joinChannel(Channel &target);
		void					leaveChannel(Channel &target);
		bool					operator==(const Client& rhs) const;

};
