/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocyn <ocyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 17:10:40 by ocyn              #+#    #+#             */
/*   Updated: 2024/09/24 18:34:42 by ocyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

std::string intToString(int value) {
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

int	ft_stoi(const string &str)
{
	int	out;
	std::stringstream	ss(str);

	ss >> out;
	return out;
}

std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> result;
	std::stringstream ss (s);
	std::string item;

	while (getline (ss, item, delim)) {
		result.push_back (item);
	}

	return result;
}

bool isAuthorized(char c) {
	if (std::isalpha(c) && !std::isspace(c) && c != '#' && c != '&' && c != ':') 
		return true;
	return false;
}

/*
	@brief Check if the port is valid (digits; no leading 0; in the range of 0 to 65535)
	@param	port The port to check
	@return	True if the port is valid, false otherwise
*/
bool isPortValid(const std::string &port)
{
	if (port.empty())
		return false;
	// Check if all characters are digits
	for (std::string::const_iterator it = port.begin(); it != port.end(); ++it) {
		if (!std::isdigit(*it))
			return false;
	}
	// Check if there is a leading 0
	if (port.size() > 1 && port[0] == '0')
		return false;
	// Check if the port is in the range of 0 to 65535
	if (ft_stoi(port) < 0 || ft_stoi(port) > 65535)
		return false;
	return true;
}
