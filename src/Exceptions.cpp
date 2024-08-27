/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammirat <aammirat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:43:16 by ocyn              #+#    #+#             */
/*   Updated: 2024/08/27 14:33:01 by aammirat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Exceptions.hpp"

const char * SocketCreationException::what() const throw() {

	string  error = "Error while creating socket";
	const char * ret;
	if (errno != 0) {
		error += ": ";
		error += strerror(errno);
	}
	ret = error.c_str();
	return ret;
}

const char * SocketBindException::what() const throw() {

	return "Error while binding the socket\n";
}

const char * SocketListenException::what() const throw() {

	string  error = "Error while listening on the socket";
	const char * ret;
	if (errno != 0) {
		error += ": ";
		error += strerror(errno);
	}
	ret = error.c_str();
	return ret;
}

