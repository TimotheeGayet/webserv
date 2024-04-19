// Server.hpp

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <signal.h>
#include <cstdlib>
#include "../config/GlobalConfig.hpp"
#include "../cgi/CgiHandler.hpp"

struct SocketInfo {
    int socket_fd;
    u_int16_t port;
};

class Server {
    public:
        Server(GlobalConfig conf, std::vector<u_int16_t> ports);
        ~Server();

        class ExitSignal : public std::exception {
            public:
                const char* what() const throw() { return " signal received!"; }
        };
        class ExitError : public std::exception {
            public:
                const char* what() const throw() { return "exit caused by an error..."; }
        };
        
        int                     run();

    private:
        SocketInfo initializeSocket(u_int16_t port);

        GlobalConfig                  _GlobalConfig;
        std::vector<u_int16_t>  _ports;
};