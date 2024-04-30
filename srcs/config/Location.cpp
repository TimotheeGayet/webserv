// Location.cpp

#include "../../includes/config/Location.hpp"

// UTILS
// ************************************************************************************************ //

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
    return errorPages;
}

// ************************************************************************************************ //

Location::Location()
    : _autoindex(false), 
      _path(""), 
      _root(""), 
      _index(""), 
      _redirect_url(""), 
      _client_body_temp_path(""), 
      _client_max_body_size("0"), 
      _allowed_methods(std::vector<std::string>()), 
      _error_pages(std::map<int, std::string>()) {}

Location::~Location() {}

void    Location::setLocationParam(const std::string key, const std::string value) {
    if (key == "autoindex") {
        this->setAutoindex(value);
    }
    else if (key == "path") {
        if (value[value.size() - 1] == '/' && value.size() > 1){
            this->setPath(value.substr(0, value.size() - 1));
        }
        else {
            this->setPath(value);
        }
    }
    else if (key == "root") {
        if (value[value.size() - 1] == '/' && value.size() > 1){
            this->setRoot(value.substr(0, value.size() - 1));
        }
        else {
            this->setRoot(value);
        }
    }
    else if (key == "index") {
        this->setIndex(value);
    }
    else if (key == "redirect_url") {
        this->setRedirectUrl(value);
    }
    else if (key == "client_body_temp_path") {
        this->setClientBodyTempPath(value);
    }
    else if (key == "client_max_body_size") {
        this->setClientMaxBodySize(value);
    }
    else if (key == "allowed_methods") {
        this->setAllowedMethods(value);
    }
    else if (key == "error_pages") {
        try {
            this->setErrorPages(get_error_pages(value));
        }
        catch (const std::exception& e) {
            throw std::runtime_error("Error while reading error pages : " + std::string(e.what()));
        }
    }
    else {
        throw std::runtime_error("Unknown key in location : " + key);
    }
}

void Location::setAutoindex(const std::string value) {
    if (value == "on") {
        _autoindex = true;
    } else if (value == "off") {
        _autoindex = false;
    } else {
        throw std::invalid_argument("Error: '" + value + "' is not a valid value (must be on / off).");
    }
}

void Location::setPath(const std::string value) {
    _path = value;
}

void Location::setRoot(const std::string value) {
    _root = value;
}

void Location::setIndex(const std::string value) {
    _index = value;
}

void Location::setRedirectUrl(const std::string value) {
    _redirect_url = value;
}

void Location::setClientBodyTempPath(const std::string value) {
    _client_body_temp_path = value;
}

void Location::setClientMaxBodySize(const std::string value) {
    _client_max_body_size = value;
}

void Location::setAllowedMethods(const std::string value) {
    std::istringstream iss(value);
    std::string method;
    while (iss >> method) {
        if (method == "POST" || method == "GET" || method == "DELETE") {
            _allowed_methods.push_back(method);
        }
        else {
            throw std::invalid_argument("Error: '" + method + "' is not an allowed method (GET, POST, DELETE).");
        }
    }
}

void Location::setErrorPages(const std::map<int, std::string> value) {
    _error_pages = value;
}

bool Location::isConfigured() const {
    if (!_root.empty() && !_path.empty()) {
        return true;
    }
    else {
        return false;
    }
}

bool Location::getAutoindex() const {
    return _autoindex;
}

std::string Location::getPath() const {
    return _path;
}

std::string Location::getRoot() const {
    return _root;
}

std::string Location::getIndex() const {
    return _index;
}

std::string Location::getRedirectUrl() const {
    return _redirect_url;
}

std::string Location::getClientMaxBodySize() const {
    return _client_max_body_size;
}

std::string Location::getClientBodyTempPath() const {
    return _client_body_temp_path;
}

std::map<int, std::string> Location::getErrorPages() const {
    return _error_pages;
}

std::vector<std::string> Location::getAllowedMethods() const {
    return _allowed_methods;
}
