/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   Client.hpp										 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: lcamerly <lcamerly@student.42lyon.fr>	  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/08/14 11:44:58 by ocyn			  #+#	#+#			 */
/*   Updated: 2024/09/24 01:19:28 by lcamerly		 ###   ########.fr	   */
/*																			*/
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
		int	 				_clientFd;
		int 				_ispassgiven;
		bool				_isRegistered;
		string  			_nickname;
		string  			_username;
		string  			_realname;
		string  			_password;
		string				_command;
		string				_modstring;
		vector<Channel *> 	_joinedChannels;
		
	public :
		Client(int fd);
		~Client();

		bool				getIspassgiven() const;
		bool				getRegistrationStatus() const;
		bool				operator==(const Client& rhs) const;
		void				setRegistrationStatus(bool status);
		void				setNickname(string nickname);
		void				setUsername(string username);
		void				setCommand(string command);
		void				setRealname(string realname);
		void				setIspassgiven(bool a);
		void				setPassword(string password);
		void				joinChannel(Channel &target);
		void				leaveChannel(Channel &target);
		void				clearCommand();
		void				appendCommand(string command);

		int					getClientFd() const;
		string				getNickname() const;
		string				getCommand() const;
		string				getUsername() const;
		string				getRealname() const;
		string				getHostname() const;
		string				getPassword() const;
		vector<Channel *>   getJoinedChannels();
		Channel				*findChannel(string Name);
};