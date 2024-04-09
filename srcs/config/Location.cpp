// Location.cpp

#include "../../includes/config/Location.hpp"

// ************************************************************************************************ //

static std::map<int, std::string> get_error_pages(const std::string &value) {
    std::map<int, std::string> errorPages;
    std::istringstream iss(value);
    std::string token;

    while (iss >> token) {
        size_t pos = token.find(':');
        if (pos == std::string::npos) {
            // throw error
        }
        try {
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
                // throw error
            }
            std::string page = token.substr(pos + 1);
            errorPages[code] = page;
        }
        catch (const std::exception& e) {
            // throw error
        }
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

void    Location::setLocationParam(const std::string &key, const std::string &value) {
    if (key == "autoindex") {
        this->setAutoindex(value);
    }
    else if (key == "path") {
        this->setPath(value);
    }
    else if (key == "root") {
        this->setRoot(value);
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
            throw std::runtime_error("Erreur lors de la lecture des pages d'erreur : " + std::string(e.what()));
        }
    }
    else {
        throw std::runtime_error("Clé inconnue dans la configuration d'une location : " + key);
    }
}

std::vector<std::string> Location::getAllowedMethods() const {
    return _allowed_methods;
}

void Location::setAutoindex(const std::string &value) {
    if (value == "on") {
        _autoindex = true;
    } else if (value == "off") {
        _autoindex = false;
    } else {
        throw std::invalid_argument("Erreur: '" + value + "' n'est pas une valeur valide pour autoindex. Les valeurs valides sont 'on' et 'off'.");
    }
}

void Location::setPath(const std::string &value) {
    _path = value;
}

void Location::setRoot(const std::string &value) {
    _root = value;
}

void Location::setIndex(const std::string &value) {
    _index = value;
}

void Location::setRedirectUrl(const std::string &value) {
    _redirect_url = value;
}

void Location::setClientBodyTempPath(const std::string &value) {
    _client_body_temp_path = value;
}

void Location::setClientMaxBodySize(const std::string &value) {
    _client_max_body_size = value;
}

void Location::setAllowedMethods(const std::string &value) {
    std::istringstream iss(value);
    std::string method;
    while (iss >> method) {
        if (method == "POST" || method == "GET" || method == "DELETE") {
            _allowed_methods.push_back(method);
        }
        else {
            throw std::invalid_argument("Erreur: '" + method + "' n'est pas une méthode autorisée. Les méthodes autorisées sont 'POST', 'GET' et 'DELETE'.");
        }
    }
}

void Location::setErrorPages(const std::map<int, std::string> &value) {
    _error_pages = value;
}

