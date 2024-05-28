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
#include "../server/SocketInfo.hpp"
#include "../config/GlobalConfig.hpp"
#include "../cgi/CgiHandler.hpp"
#include "../request/Request.hpp"
#include "../response/Response.hpp"

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
        SocketInfo              initializeSocket(u_int16_t port);
        void                    listenInit();
        uint16_t                findPort(int &fd);
        void                    newConnection(int fd);
        void                    existingConnection(int fd, std::map<int, std::string> &requests);
        void                    handleDisconnect(int fd, std::map<int, std::string> &requests, int bytes_received);
        void                    handleRequest(int fd, std::map<int, std::string> &requests, const std::string& request);
        void                    handleResponse(int fd, std::map<int, std::string> &requests, const std::string& response, Request &req, Response &res);
        

        GlobalConfig            _GlobalConfig;
        std::vector<u_int16_t>  _ports;
};