#pragma once

#include "Server.hpp"



class SocketCreationException : public std::exception { public : const char * what() const throw();};
class SocketBindException : public std::exception { public : const char * what() const throw();};
class SocketListenException : public std::exception { public : const char * what() const throw();};