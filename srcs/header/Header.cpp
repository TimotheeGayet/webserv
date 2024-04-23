#include "../../includes/header/Header.hpp"
#include <iostream>

HeaderRequest::HeaderRequest() {
}

HeaderRequest::~HeaderRequest() {}

void HeaderRequest::removeHeader(const std::string &key) {
    _headers.erase(key);
}

bool HeaderRequest::isSet(const std::string &key) const {
    return !_headers.at(key).empty();
}

int HeaderRequest::size() const {
    return _headers.size();
}

std::string HeaderRequest::getHeader(const std::string &key) const {
    return _headers.at(key);
}

std::map<std::string, std::string> HeaderRequest::getHeaders() const {
    return _headers;
}

void HeaderRequest::addHeader(const std::string &key, const std::string &value) {
    _headers[key] = value;
}

void HeaderRequest::updateHeader(const std::string &key, const std::string &value) {
    _headers[key] = value;
}

int HeaderRequest::getIndex(const std::string &key) const {
    int index = 0;
    for (std::map<std::string, std::string>::const_iterator it = this->_headers.begin(); it != this->_headers.end(); it++) {
        if (it->first == key)
            return index;
        index++;
    }
    return -1;
}

// ************************************************************

HeaderResponse::HeaderResponse() {
}

HeaderResponse::~HeaderResponse() {}

void HeaderResponse::removeHeader(const std::string &key) {
    _headers.erase(key);
}

bool HeaderResponse::isSet(const std::string &key) const {
    return !_headers.at(key).empty();
}

int HeaderResponse::size() const {
    return _headers.size();
}

std::string HeaderResponse::getHeader(const std::string &key) const {
    return _headers.at(key);
}

std::map<std::string, std::string> HeaderResponse::getHeaders() const {
    return _headers;
}

void HeaderResponse::addHeader(const std::string &key, const std::string &value) {
    _headers[key] = value;
}

void HeaderResponse::updateHeader(const std::string &key, const std::string &value) {
    _headers[key] = value;
}

int HeaderResponse::getIndex(const std::string &key) const {
    int index = 0;
    for (std::map<std::string, std::string>::const_iterator it = this->_headers.begin(); it != this->_headers.end(); it++) {
        if (it->first == key)
            return index;
        index++;
    }
    return -1;
}

