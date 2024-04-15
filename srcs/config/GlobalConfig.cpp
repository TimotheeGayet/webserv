// GlobalConfig.cpp

#include "../../includes/config/GlobalConfig.hpp"

// UTILS
// ************************************************************************************************ //

static bool is_whitespace(const char c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

static std::string parseSection(const std::string& line) {
    size_t endPos = line.find_last_of(']');
    if (endPos == std::string::npos) {
        throw std::runtime_error("Bad format : " + line);
    }
    return line.substr(1, endPos - 1);
}

// ************************************************************************************************ //

GlobalConfig::GlobalConfig() {}

GlobalConfig::GlobalConfig(const GlobalConfig &other)
    : _servers(other._servers) {}

GlobalConfig &GlobalConfig::operator=(const GlobalConfig &other) {
    if (this != &other) {
        _servers = other._servers;
    }
    return *this;
}

GlobalConfig::GlobalConfig(const std::string &path)
    : _servers()
{
    std::ifstream file(path.c_str());
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open the GlobalConfig file : " + path);
    }

    std::string line;
    std::string section("");
    bool emptyField = false;
    ServerConfig *currentServer = NULL;

    while (std::getline(file, line) && !file.eof()) {
        if (line.empty() || line[0] == '#' || is_whitespace(line[0])) {
            continue;
        }

        if (line[0] == '[') {
            if (emptyField) {
                throw std::runtime_error("GlobalConfiguration error : empty section");
            }
            section = parseSection(line);
            if (section == "server") {
                if (currentServer != NULL && !currentServer->isGlobalConfigured()) {
                    throw std::runtime_error("GlobalConfiguration error : 'server' section without GlobalConfiguration");
                }
                _servers.push_back(ServerConfig());
                currentServer = &_servers.back();
            }
            emptyField = true;
            continue;
        }

        if (section == "server") {
            if (currentServer == NULL) {
                throw std::runtime_error("GlobalConfiguration error : 'server' section before server definition");
            }
            currentServer->parseServerConfig(line);
            emptyField = false;
        }
        else if (section == "location") {
            if (currentServer == NULL) {
                throw std::runtime_error("GlobalConfiguration error : 'location' section before server definition");
            }
            if (currentServer->parseLocations(file, line, section) == 1) {
                if (currentServer != NULL && !currentServer->isGlobalConfigured()) {
                    throw std::runtime_error("GlobalConfiguration error : 'server' section without GlobalConfiguration");
                }
                _servers.push_back(ServerConfig());
                currentServer = &_servers.back();
                emptyField = true;
            }
            else {
                emptyField = false;
            }
        }
        else {
            throw std::runtime_error("Unkown section in GlobalConfig file : " + section);
        }
    }

    for (std::vector<ServerConfig>::iterator it = _servers.begin(); it != _servers.end(); ++it) {
        if (!it->isGlobalConfigured()) {
            throw std::runtime_error("GlobalConfiguration error : 'server' section without GlobalConfiguration");
        }
    }
}

GlobalConfig::~GlobalConfig() {}

std::vector<ServerConfig> GlobalConfig::getServerConfigs() const {
    return _servers;
}

DefaultErrors GlobalConfig::getDefaultErrors() const {
    return _defaultErrors;
}

void GlobalConfig::printGlobalConfig() const {

    std::cout << "GlobalConfig:" << std::endl;
    for (std::vector<ServerConfig>::const_iterator it = _servers.begin(); it != _servers.end(); ++it) {
        std::cout << "Server GlobalConfig:" << std::endl;
        std::cout << "  Port: " << it->getPort() << std::endl;
        std::cout << "  Root: " << it->getRoot() << std::endl;
        std::cout << "  Server name: " << it->getServerName() << std::endl;
        std::cout << "  Client max body size: " << it->getClientMaxBodySize() << std::endl;
        std::cout << "  Error pages:" << std::endl;
        const std::map<int, std::string>& errorPages = it->getErrorPages();
        for (std::map<int, std::string>::const_iterator it2 = errorPages.begin(); it2 != errorPages.end(); ++it2) {
            std::cout << "    " << it2->first << ": " << it2->second << std::endl;
        }
        std::cout << "  Locations:" << std::endl;
        const std::vector<Location>& locations = it->getLocations();
        for (std::vector<Location>::const_iterator it2 = locations.begin(); it2 != locations.end(); ++it2) {
            std::cout << "    Location:" << std::endl;
            std::cout << "      Autoindex: " << it2->getAutoindex() << std::endl;
            std::cout << "      Path: " << it2->getPath() << std::endl;
            std::cout << "      Root: " << it2->getRoot() << std::endl;
            std::cout << "      Index: " << it2->getIndex() << std::endl;
            std::cout << "      Redirect URL: " << it2->getRedirectUrl() << std::endl;
            std::cout << "      Client body temp path: " << it2->getClientBodyTempPath() << std::endl;
            std::cout << "      Client max body size: " << it2->getClientMaxBodySize() << std::endl;
            std::cout << "      Allowed methods:" << std::endl;
            size_t size = 0;
            std::vector<std::string> allowedMethods = it2->getAllowedMethods();
            size = allowedMethods.size();
            for (size_t i = 0; i < size; i++) {
                std::cout << "        " << allowedMethods[i];
                if (i < size - 1) {
                    std::cout << ", ";
                }
                else {
                    std::cout << std::endl;
                }
            }
            std::cout << "      Error pages:" << std::endl;
            const std::map<int, std::string>& locationErrorPages = it2->getErrorPages();
            for (std::map<int, std::string>::const_iterator it3 = locationErrorPages.begin(); it3 != locationErrorPages.end(); ++it3) {
                std::cout << "        " << it3->first << ": " << it3->second << std::endl;
            }
        }
    }
}