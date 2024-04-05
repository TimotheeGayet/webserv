// Config.hpp

#pragma once

#include <map>
#include <string>
#include <iostream>
#include "../../includes/config/Location.hpp"

class Location;

class Config {
    public:
        Config(const std::string &path);
        ~Config();

    private:
        int                         _port     ;             // port to map
        Location*                   _locations;             // locations list
        std::string                 _server_name;           // server adress
        std::string                 _client_max_body_size;  // body size limitaion
        std::map<int, std::string>  _error_pages;           // map of error pages with their codes
};