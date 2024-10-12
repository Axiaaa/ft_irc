#pragma once

#include <iostream> 
#include <cstring>
#include <sstream>


std::vector<std::string>            split(const std::string &s, char delim);
std::string                         intToString(int value);
vector<char>                        charsplit(const string &s);
bool                                isAuthorized(char c);
void	                            ft_log(string content);
std::pair<std::string, std::string> splitFirstSpace(const std::string &s);
std::vector<std::pair<std::string, std::string> > bufferParser(const std::string& buffer);

#define BLUE    "\033[1;34m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define CYAN    "\033[1;36m"
#define MAGENTA "\033[1;35m"
#define RESET   "\033[0m"

