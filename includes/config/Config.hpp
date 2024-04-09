// Config.hpp

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

class Config {
    public:
        Config(const std::string &path);
        ~Config();

        std::string parseSection(const std::string &line);
        void        parseServerConfig(const std::string &line);
        void        parseLocationConfig(std::ifstream& file, const std::string& firstLine);

    private:
        int                         _port;                  // port to map
        std::vector<Location>       _locations;             // locations list
        std::string                 _root;                  // path of the folder to route
        std::string                 _server_name;           // server adress
        std::string                 _client_max_body_size;  // body size limitaion
        std::map<int, std::string>  _error_pages;           // map of error pages with their codes
};