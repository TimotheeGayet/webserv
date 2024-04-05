// Config.hpp

#pragma once

#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "../../includes/config/Location.hpp"

class Location;

class Config {
    public:
        Config(const std::string &path);
        ~Config();

    private:
        int                         _port;                  // port to map
        std::vector<Location>       _locations;             // locations list
        std::string                 _server_name;           // server adress
        std::string                 _client_max_body_size;  // body size limitaion
        std::map<int, std::string>  _error_pages;           // map of error pages with their codes
};