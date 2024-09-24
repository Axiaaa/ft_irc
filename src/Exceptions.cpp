/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcamerly <lcamerly@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:43:16 by ocyn              #+#    #+#             */
/*   Updated: 2024/09/24 00:24:50 by lcamerly         ###   ########.fr       */
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

const char * SigInt::what() const throw() {

	return "SIGINT received";
}