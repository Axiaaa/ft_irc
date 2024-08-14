#pragma once

#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <sys/wait.h>
#include <iterator>
#include "Exceptions.hpp"

using std::string;
using std::vector;

class Server {


    private :
        sockaddr_in addr_;
        vector<int> clientsList_;
        fd_set fdSet_;
        int socket_;

    public :

        Server(char *port, string password);
        ~Server();
        void createSocket();
        void bindSocket();
        void listenSocket();

        int& getSocket();
        sockaddr getAddr();
        fd_set& getFdSet();
        vector<int>& getClientsList();

};