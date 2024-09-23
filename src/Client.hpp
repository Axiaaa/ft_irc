/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcamerly <lcamerly@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:44:58 by ocyn              #+#    #+#             */
/*   Updated: 2024/09/23 11:59:55 by lcamerly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"
#include "Exceptions.hpp"
#include "time.h"

using std::string;
using std::vector;

class Channel;

class Client {

    private : 
        time_t  creationTime_;
        string  nickname_;
        string  username_;
        string  realname_;
        string  password_;
        int     clientFd_;
        int ispassgiven_;
        bool    isRegistered_;
        sockaddr_in addr_;
        vector<Channel *> joinedChannels_;
        
	public :
		Client(int fd, time_t t);
		~Client();

        string  getNickname() const;
        string  getUsername() const;
        string  getRealname() const;
        string  getHostname() const;
        string  getPassword() const;

        bool    getIspassgiven() const;
        bool    getRegistrationStatus() const;
        bool    operator==(const Client& rhs) const;

        void    setRegistrationStatus(bool status);
        void    setNickname(string nickname);
        void    setUsername(string username);
        void    setRealname(string realname);
        void    setIspassgiven(bool a);
        void    setPassword(string password);
        void    joinChannel(Channel &target);
        void    leaveChannel(Channel &target);
        
        int     getClientFd() const;

        vector<Channel *> getJoinedChannels();

};