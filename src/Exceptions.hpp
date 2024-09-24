/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcamerly <lcamerly@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:45:03 by ocyn              #+#    #+#             */
/*   Updated: 2024/09/24 01:25:30 by lcamerly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "Server.hpp"

class SocketCreationException   :   public std::exception { public : const char * what() const throw();};
class SocketBindException       :   public std::exception { public : const char * what() const throw();};
class SocketListenException     :   public std::exception { public : const char * what() const throw();};
class SendFailedException       :   public std::exception { public : const char * what() const throw();};
class SigInt                    :   public std::exception { public : const char * what() const throw();};
class ChannelNotExistingException	:   public std::exception { public : const char * what() const throw();};
class AlreadyInChannelException		:   public std::exception { public : const char * what() const throw();};
class InvalidChannelName			:   public std::exception { public : const char * what() const throw();};