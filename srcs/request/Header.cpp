#include "../../includes/request/Header.hpp"
#include <iostream>

Header::Header() {
}

Header::~Header() {}

void Header::removeHeader(const std::string &key) {
    _headers.erase(key);
}

bool Header::isSet(const std::string &key) const {
    return !_headers.at(key).empty();
}

int Header::size() const {
    return _headers.size();
}

std::string Header::getHeader(const std::string &key) const {
    return _headers.at(key);
}

std::map<std::string, std::string> Header::getHeaders() const {
    return _headers;
}

void Header::addHeader(const std::string &key, const std::string &value) {
    _headers[key] = value;
}

void Header::updateHeader(const std::string &key, const std::string &value) {
    _headers[key] = value;
}

int Header::getIndex(const std::string &key) const {
    int index = 0;
    for (std::map<std::string, std::string>::const_iterator it = this->_headers.begin(); it != this->_headers.end(); it++) {
        if (it->first == key)
            return index;
        index++;
    }
    return -1;
}