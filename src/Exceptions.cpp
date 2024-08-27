/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocyn <ocyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:43:16 by ocyn              #+#    #+#             */
/*   Updated: 2024/08/27 17:30:25 by ocyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Exceptions.hpp"

const char * SocketCreationException::what() const throw() {

	string error = "Error while creating socket";
	if (errno != 0) {
		error += ": ";
		error += strerror(errno);
	}
	return error.c_str();
}

const char * SocketBindException::what() const throw() {

	return "Error while binding the socket\n";
}

const char * SocketListenException::what() const throw() {

	string error = "Error while listening on the socket";
	if (errno != 0) {
		error += ": ";
		error += strerror(errno);
	}
	return error.c_str();
}

const char * SendFailedException::what() const throw() {

    string error = "Error while sending data with send()";
    if (errno != 0) {
        error += ": ";
        error += strerror(errno);
    }
    return error.c_str();
}

const char * ChannelNotExistingException::what() const throw() {

	return "Error while creating / finding the channel, specified channel not exist\n";
}

const char * AlreadyInChannelException::what() const throw() {

	return "Error while joining the specified channel, the client is already in the channel\n";
}

const char * InvalidChannelName::what() const throw() {

	return "Error while creating / finding the channel, specified name not valid\n";
}
