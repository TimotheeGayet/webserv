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
#include "../config/Config.hpp"

struct SocketInfo {
    int socket_fd;
    u_int16_t port;
};

class Server {
    public:
        Server(Config conf, std::vector<u_int16_t> ports);
        ~Server();

        int                     run();

    private:
        SocketInfo initializeSocket(u_int16_t port);

        Config                  _config;
        std::vector<u_int16_t>  _ports;
};