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

std::string Request::getFile()
{
	return this->_file;
}

int Request::getReturnCode() const {
	return this->_return_code;
}

std::string Request::getPath() {
	return this->_path;
}

ServerConfig Request::getServerConfig() const {
	return this->_server_config;
}