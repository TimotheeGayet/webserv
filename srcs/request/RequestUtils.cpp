#include "../../includes/Globals.hpp"
#include "../../includes/request/Request.hpp"
#include "../../includes/header/Header.hpp"
#include "../../includes/config/ServerConfig.hpp"
#include <algorithm>
#include <sys/stat.h>
#include <string>

std::string Request::getResourceType() {
    struct stat fileStat;
    if (stat(this->_path.c_str(), &fileStat) == 0) {
        if (S_ISREG(fileStat.st_mode)) {
            return "file";
        } else if (S_ISDIR(fileStat.st_mode)) {
            return "directory";
        }
    }

    if (this->_path == "/")
        return "root";
    return "unknown";
}

long Request::stringToLong(const std::string& str) {
    long result = 0;
    int sign = 1;
    int i = 0;

    if (str[0] == '-') {
        sign = -1;
        i++;
    }

    for (; static_cast<size_t>(i) < str.length(); i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            result = result * 10 + (str[i] - '0');
        } else {
            return 0;
        }
    }

    return sign * result;
}

void Request::bodyParsing()
{
    this -> _body = this->_req.substr(this->_req.find("\r\n") + 2);
    if (this->_headers.getHeader("Content-Length") == "")
    {
        this->_return_code = 411;
        throw std::runtime_error("Content-Length header is missing");
    }
    if (this->_body.length() < static_cast<size_t>(this->stringToLong(this->_headers.getHeader("Content-Length"))))
    {
        this->_return_code = 400;
        throw std::runtime_error("Invalid body length");
    }
    if (this->_headers.getHeader("Transfer-Encoding") == "chunked")
    {
        std::string chunked = this->_body;
        std::string body = "";
        while (chunked.length() > 0)
        {
            size_t chunkSize = this->stringToLong(chunked.substr(0, chunked.find("\r\n")));
            if (chunkSize == 0)
                break;
            body += chunked.substr(chunked.find("\r\n") + 2, chunkSize);
            chunked = chunked.substr(chunked.find("\r\n") + 2 + chunkSize);
        }
        this->_body = body;
    } else {
        this->_body = this->_body.substr(0, this->stringToLong(this->_headers.getHeader("Content-Length")));
    }
    return;
}