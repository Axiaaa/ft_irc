/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocyn <ocyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:45:03 by ocyn              #+#    #+#             */
/*   Updated: 2024/08/27 17:27:58 by ocyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "Server.hpp"

class SocketCreationException		:   public std::exception { public : const char * what() const throw();};
class SocketBindException			:   public std::exception { public : const char * what() const throw();};
class SocketListenException			:   public std::exception { public : const char * what() const throw();};
class SendFailedException			:   public std::exception { public : const char * what() const throw();};
class ChannelNotExistingException	:   public std::exception { public : const char * what() const throw();};
class AlreadyInChannelException		:   public std::exception { public : const char * what() const throw();};
class InvalidChannelName			:   public std::exception { public : const char * what() const throw();};