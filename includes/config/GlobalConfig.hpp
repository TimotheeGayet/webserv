// GlobalConfig.hpp

#pragma once

#include <vector>
#include <string>
#include "./ServerConfig.hpp"
#include "../server/DefaultErrors.hpp"

class GlobalConfig {
    public:
        GlobalConfig();
        GlobalConfig(const std::string &path);
        GlobalConfig &operator=(const GlobalConfig &other);
        GlobalConfig(const GlobalConfig &other);
        ~GlobalConfig();

        void                        printGlobalConfig() const;
        std::vector<ServerConfig>   getServerConfigs() const;
        DefaultErrors               getDefaultErrors() const;

    private:
        std::vector<ServerConfig>   _servers;
        DefaultErrors               _defaultErrors;
};