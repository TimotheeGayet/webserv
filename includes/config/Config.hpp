// Config.hpp

#pragma once

#include <vector>
#include <string>
#include "../../includes/config/ServerConfig.hpp"

class Config {
    public:
        Config();
        Config(const std::string &path);
        Config(const Config &other);
        Config &operator=(const Config &other);
        ~Config();

        std::string                 parseSection(const std::string &line);
        std::vector<ServerConfig>   getServerConfigs() const;
        void                        printConfig() const;

    private:
        std::vector<ServerConfig> _servers;
};