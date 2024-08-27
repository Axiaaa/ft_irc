#include "Server.hpp"

std::string intToString(int value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
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