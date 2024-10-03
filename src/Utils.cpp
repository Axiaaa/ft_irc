#include "Server.hpp"

// Convert int to string
std::string intToString(int value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

// Split string by a single char delimiter
std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss (s);
    std::string item;

    while (getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}

// Check if a char is authorized
bool isAuthorized(char c) {
    if (c >= 'a' && c <= 'z')
        return true;
    if (c >= 'A' && c <= 'Z')
        return true;
    if (c >= '0' && c <= '9')
        return true;
    if (c == '[' || c == ']' || c == '\\' || c == '|' || c == '{' || c == '}')
        return true;
    return false;
}

//Split a string by the first space and retuns a pair of strings
std::pair<std::string, std::string> splitFirstSpace(const std::string &s) {
    std::string command;
    std::string arg;
    std::vector<std::string> split_buffer = split(s, ' ');

    if (split_buffer.size() > 1) {
        command = split_buffer[0];
        arg = s.substr(s.find(' ') + 1);
    } else {
        command = s;
    }
    return std::make_pair(command, arg);
}


vector <char> charsplit(const string &s)
{
    vector <char> result;
    for (int i = 0; i < (int)s.size(); i++)
    {
        result.push_back(s[i]);
    }
    return result;
}