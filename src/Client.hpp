/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcamerly <lcamerly@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:44:58 by ocyn              #+#    #+#             */
/*   Updated: 2024/08/20 17:45:49 by lcamerly         ###   ########.fr       */
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
        int long long id_;
        
	public :
		Client(int fd, sockaddr_in addr, time_t t);
		~Client();
        string  getNickname() const;
        string  getUsername() const;
        string  getRealname() const;
        string  getHostname() const;
        bool    getRegistrationStatus();
        void    setRegistrationStatus(bool status);
        void    setNickname(string nickname);
        void    setUsername(string username);
        void    setRealname(string realname);
        void    setId(int long long id);
        int     getClientFd() const;
        void    init(string buffer, Server& server);
        int long long getId() const;
        bool    operator==(const Client& rhs) const;

};