// GlobalConfig.hpp

#pragma once

#include <vector>
#include <string>
#include "../../includes/config/ServerConfig.hpp"

class GlobalConfig {
    public:
        GlobalConfig();
        GlobalConfig(const std::string &path);
        GlobalConfig &operator=(const GlobalConfig &other);
        GlobalConfig(const GlobalConfig &other);
        ~GlobalConfig();

        void                        printGlobalConfig() const;
        std::vector<ServerConfig>   getServerConfigs() const;

    private:
        std::vector<ServerConfig> _servers;
};