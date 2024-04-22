// ServerConfig.hpp

#pragma once

#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include "../../includes/config/Location.hpp"

class Location;

class ServerConfig {
    public:
        ServerConfig();
        ~ServerConfig();

        bool                        isConfigured() const;
        void                        parseServerConfig(const std::string &line);
        int                         parseServerLocation(std::ifstream& file, const std::string& firstLine, std::string& section);

        u_int16_t                   getPort() const;
        std::vector<Location>       getLocations() const;
        std::string                 getRoot() const;
        std::string                 getServerName() const;
        std::string                 getClientMaxBodySize() const;
        std::map<int, std::string>  getErrorPages() const;
    
    private:
        u_int16_t                    _port;                 // port to map
        std::vector<Location>       _locations;             // locations list
        std::string                 _root;                  // path of the folder to route
        std::string                 _server_name;           // server adress
        std::string                 _client_max_body_size;  // body size limitaion
        std::map<int, std::string>  _error_pages;           // map of error pages with their codes
};
