/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcamerly <lcamerly@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:43:16 by ocyn              #+#    #+#             */
/*   Updated: 2024/10/15 15:00:04 by lcamerly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Exceptions.hpp"


const char * SigInt::what() const throw()
{
	return "SIGINT received";
}

const char * SocketCreationException::what() const throw() {

	string error = "Error while creating socket";
	const char *ret;
	if (errno != 0) {
		error += ": ";
		error += strerror(errno);
	}
	ret = error.c_str();
	return (ret);
}

const char * SocketBindException::what() const throw() {

	return "Error while binding the socket\n";
}

const char * SocketListenException::what() const throw() {

	string error = "Error while listening on the socket";
	const char *ret;
	if (errno != 0) {
		error += ": ";
		error += strerror(errno);
	}
	ret = error.c_str();
	return (ret);
}

const char * SendFailedException::what() const throw() {

    string error = "Error while sending data with send()";
	const char *ret;
    if (errno != 0) {
        error += ": ";
        error += strerror(errno);
    }
	ret = error.c_str();
	return (ret);
}