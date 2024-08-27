/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocyn <ocyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:44:58 by ocyn              #+#    #+#             */
/*   Updated: 2024/08/27 17:04:22 by ocyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"
#include "Exceptions.hpp"
#include "time.h"

using std::string;
using std::vector;

class Server;

class Client {

	private : 
		string  nickname_;
		string  username_;
		string  realname_;
		time_t  creationTime_;
		string  password_;
		int     clientFd_;
		bool    isRegistered_;
		sockaddr_in addr_;
		
	public :
		Client(int fd, sockaddr_in addr, time_t t);
		~Client();
		string  getNickname() const;
		string  getUsername() const;
		string  getRealname() const;
		string  getHostname() const;
		bool    getRegistrationStatus() const;
		void    setRegistrationStatus(bool status);
		void    setNickname(string nickname);
		void    setUsername(string username);
		void    setRealname(string realname);
		int     getClientFd() const;
		bool    operator==(const Client& rhs) const;

};