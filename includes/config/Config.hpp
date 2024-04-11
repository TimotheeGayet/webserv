// Config.hpp

#pragma once

#include <vector>
#include <string>
#include "../../includes/config/ServerConfig.hpp"

class Config {
    public:
        Config();
        Config(const std::string &path);
        Config &operator=(const Config &other);
        Config(const Config &other);
        ~Config();

        void                        printConfig() const;
        std::vector<ServerConfig>   getServerConfigs() const;

    private:
        std::vector<ServerConfig> _servers;
};