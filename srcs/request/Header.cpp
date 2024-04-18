#include "../../includes/request/Header.hpp"

Header::Header() {
    this->_headers["Host"] = "";
    this->_headers["Content-Length"] = "";
    this->_headers["Transfer-Encoding"] = "";
    this->_headers["Content-Type"] = "";
    this->_headers["User-Agent"] = "";
    this->_headers["Accept"] = "";
    this->_headers["Accept-Language"] = "";
    this->_headers["Accept-Encoding"] = "";
    this->_headers["Connection"] = "";
    this->_headers["Referer"] = "";
    this->_headers["Content-Encoding"] = "";
    this->_headers["Content-Language"] = "";
    this->_headers["Content-Location"] = "";
}

Header::~Header() {}

void Header::updateHeader(const std::string &key, const std::string &value) {
    _headers[key] = value;
}

void Header::removeHeader(const std::string &key) {
    _headers.erase(key);
}

std::string Header::getHeader(const std::string &key) const {
    return _headers.at(key);
}

int Header::getIndex(const std::string &key) const {
    int index = 0;
    for (std::map<std::string, std::string>::const_iterator it = this->_headers.begin(); it != this->_headers.end(); ++it) {
        if (it->first == key)
            return index;
        index++;
    }
    return -1;
}

bool Header::isSet(const std::string &key) const {
    return !_headers.at(key).empty();
}

int Header::size() const {
    return _headers.size();
}

std::map<std::string, std::string> Header::getHeaders() const {
    return _headers;
}

std::map<std::string, std::string>::iterator Header::begin() {
    return _headers.begin();
}

std::map<std::string, std::string>::iterator Header::end() {
    return _headers.end();
}