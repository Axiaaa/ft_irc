#include "Server.hpp"

/// @brief Convert an int to a string
/// @param value The int to convert
/// @return string The string
std::string intToString(int value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

/// @brief Split a string by a delimiter
/// @param s The string to split
/// @param delim The delimiter
/// @return vector<string> The vector of strings
std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss (s);
    std::string item;

    while (getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}

/// @brief Check if a char is authorized
/// @param c The char to check
/// @return bool True if the char is authorized, false otherwise
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

// @brief Split a string by the first space
// @param s The string to split
// @return pair<string, string> The pair of strings
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
// @brief Split a string into a vector of chars
// @param s The string to split
// @return vector<char> The vector of chars
vector <char> charsplit(const string &s)
{
    vector <char> result;
    for (int i = 0; i < (int)s.size(); i++)
    {
        result.push_back(s[i]);
    }
    return result;
}

// @brief Parse a buffer and return a vector of pairs
// @param buffer The buffer to parse
// @return vector<pair<string, string> > The vector of pairs
vector<pair<string, string> >bufferParser(const string& buffer) { 
    vector<pair<string, string> > args;
    vector<string> buffsplit = split(buffer.substr(0, buffer.find(' ')), ',');
    for (size_t i = 0; i < buffsplit.size(); i++) 
    {
        pair<string, string> p(buffsplit[i], "");
        args.push_back(p);
    }
    buffsplit = split(buffer.substr(buffer.find(':') + 1, buffer.size()), ' ');
    if (buffsplit.size() == 1)
        return args;
    for (size_t i = 0; i < buffsplit.size() && i < args.size(); i++)
        args[i].second = buffsplit[i];
    return args;
}

// @brief Log a string
// @param content The string to log
void	ft_log(string content) {
	std::cout << CYAN << content << RESET << std::endl;
}