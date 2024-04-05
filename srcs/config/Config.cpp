// Config.cpp

#include "../../includes/config/Config.hpp"

Config::Config(const std::string &path) {
    std::ifstream file(path.c_str());
    if (file.is_open()) {
        std::string line;
        std::string section;
        while (std::getline(file, line)) {
            
        }
    }
    else {

    }
}

Config::~Config() {
    // Destructor
}