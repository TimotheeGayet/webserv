// ServerConfig.cpp

#include "../../includes/config/ServerConfig.hpp"

// UTILS
// ************************************************************************************************ //

static bool is_whitespace(const char c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

static bool isNumber(const std::string& str) {
    if (str.empty()) {
        return false;
    }
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
        if (!std::isdigit(*it)) {
            return false;
        }
    }
    return true;
}

static void clear_whitespace(std::string &str) {
    size_t start = 0;
    size_t end = str.size() - 1;
    while (start < str.size() && is_whitespace(str[start])) {
        start++;
    }
    while (end > 0 && is_whitespace(str[end])) {
        end--;
    }
    str = str.substr(start, end - start + 1);
}

static std::map<int, std::string> get_error_pages(const std::string &value) {
    std::map<int, std::string> errorPages;
    std::istringstream iss(value);
    std::string token;

    while (iss >> token) {
        size_t pos = token.find(':');
        if (pos == std::string::npos) {
            throw std::invalid_argument("Erreur: '" + token + "' n'est pas une page d'erreur valide.");
        }
        int code;
        std::string str = token.substr(0, pos);
        bool isNumber = true;
        for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
            if (!isdigit(*it)) {
                isNumber = false;
                break;
            }
        }
        if (isNumber) {
            code = std::atoi(str.c_str());
        } else {
            throw std::invalid_argument("Erreur: '" + str + "' n'est pas un code d'erreur valide.");
        }
        std::string page = token.substr(pos + 1);
        errorPages[code] = page;
    }
    return errorPages;
}

// ************************************************************************************************ //

ServerConfig::ServerConfig()
    : _port(0), 
      _locations(std::vector<Location>()), 
      _root(""), 
      _server_name(""), 
      _client_max_body_size("0"), 
      _error_pages(std::map<int, std::string>()) {}

ServerConfig::~ServerConfig() {}

void ServerConfig::parseServerConfig(const std::string& line) {
    std::istringstream iss(line);
    std::string key;
    std::string value;
    std::getline(iss, key, '=');
    std::getline(iss, value);
    clear_whitespace(key);
    clear_whitespace(value);

    if (key == "listen") {
        if (isNumber(value)) {
            _port = std::atoi(value.c_str());
        }
        else {
            throw std::invalid_argument("Erreur: '" + value + "' n'est pas un nombre valide pour le port.");
        }
    }
    else if (key == "root") {
        _root = value;
    }
    else if (key == "server_name") {
        _server_name = value;
    }
    else if (key == "client_max_body_size") {
        _client_max_body_size = value;
    }
    else if (key == "error_pages") {
        try {
            _error_pages = get_error_pages(value);
        }
        catch (const std::exception& e) {
            throw std::runtime_error("Erreur lors de la lecture des pages d'erreur : " + std::string(e.what()));
        }
    }
    else {
        throw std::runtime_error("Clé inconnue dans la configuration du serveur : " + key);
    }
}

void ServerConfig::parseLocations(std::ifstream& file, const std::string& firstLine) {
    Location location;

    std::string line = firstLine;
    while (!file.eof() && line[0] != '[') {
        if (!line.empty() && line[0] != '#' && !is_whitespace(line[0])) {
            std::istringstream iss(line);
            std::string key;
            std::string value;
            std::getline(iss, key, '=');
            std::getline(iss, value);
            clear_whitespace(key);
            clear_whitespace(value);
            if (!key.empty() && !value.empty()) {
                location.setLocationParam(key, value);
            }
        }
        if (!std::getline(file, line)) {
            break;
        }
    }
    if (location.isConfigured()) {
        _locations.push_back(location);
    }
    else {
        throw std::runtime_error("Erreur de configuration : location avec configuration manquante");
    }
}

bool ServerConfig::isConfigured() const {
    if (_port != 0 && !_server_name.empty() && !_locations.empty()) {
        return true;
    }
    else {
        return false;
    }
}

int ServerConfig::getPort() const {
    return _port;
}

std::vector<Location> ServerConfig::getLocations() const {
    return _locations;
}

std::string ServerConfig::getRoot() const {
    return _root;
}

std::string ServerConfig::getServerName() const {
    return _server_name;
}

std::string ServerConfig::getClientMaxBodySize() const {
    return _client_max_body_size;
}

std::map<int, std::string> ServerConfig::getErrorPages() const {
    return _error_pages;
}