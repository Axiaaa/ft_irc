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


