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
            throw std::invalid_argument("Error: '" + token + "'is not a valid Error page.");
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
            throw std::invalid_argument("Error: '" + str + "' is not a valid Error code.");
        }
        std::string page = token.substr(pos + 1);
        errorPages[code] = page;
    }

    
    for (std::map<int, std::string>::iterator it = errorPages.begin(); it != errorPages.end(); ++it) {
        std::ifstream file;
        std::string file_content;
        file.open(it->second.c_str());
        if (!file.is_open()) {
            throw std::runtime_error("Error: '" + it->second + "' is not a valid Error page.");
        }
        std::string line;
        while (std::getline(file, line) && !file.eof()) {
            file_content += line;
        }
        file.close();
        it->second = file_content;
    }
    return errorPages;
}

static std::string parseSection(const std::string& line) {
    size_t endPos = line.find_last_of(']');
    if (endPos == std::string::npos) {
        throw std::runtime_error("Bad format : " + line);
    }
    return line.substr(1, endPos - 1);
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
            _port = static_cast<u_int16_t>(std::atoi(value.c_str()));
        }
        else {
            throw std::invalid_argument("Error: '" + value + "' is not a valid port number.");
        }
    }
    else if (key == "root") {
        if (value[value.size() - 1] != '/') {
            _root = value + "/";
        }
        else {
            _root = value;
        }
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
            throw std::runtime_error("Error while reading error pages : " + std::string(e.what()));
        }
    }
    else {
        throw std::runtime_error("Unknown key in server GlobalConfiguration : " + key + ". Keys allowed are 'listen', 'root', 'server_name', 'client_max_body_size' and 'error_pages'.");
    }
}

int ServerConfig::parseServerLocation(std::ifstream& file, const std::string& firstLine, std::string& section) {
    Location    location;
    int         value;

    value = 0;
    std::string line = firstLine;
    do {
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
    } while (line[0] != '[');

    if (!line.empty() && line[0] == '[') {
        section = parseSection(line);
        if (section == "server") {
            value = 1;
        }
    }
    if (location.isConfigured()) {
        _locations.push_back(location);
    }
    else {
        throw std::runtime_error("Error: missing GlobalConfiguration in location block.");
    }
    return value;
}

bool ServerConfig::isConfigured() const {
    if (_port != 0 && !_server_name.empty() && !_locations.empty()) {
        return true;
    }
    else {
        return false;
    }
}

u_int16_t ServerConfig::getPort() const {
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