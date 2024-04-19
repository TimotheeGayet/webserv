#include "../../includes/Globals.hpp"
#include "../../includes/request/Request.hpp"
#include "../../includes/request/Header.hpp"
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

long stringToLong(const std::string& str) {
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

void Request::findHost(const std::string& value)
{
    size_t colonPos = value.find(':');
    std::string host = value.substr(0, colonPos);
    int port = -1;
    if (colonPos != std::string::npos)
        port = stringToLong(value.substr(colonPos + 1));

    const std::vector<ServerConfig>& serverConfigs = g_config.getServerConfigs();
    std::vector<ServerConfig>::const_iterator it = serverConfigs.begin();
    std::vector<ServerConfig>::const_iterator end = serverConfigs.end();
    for (; it != end; ++it)
    {
        if (it->getServerName() == host)
        {
            this->_server_config = *it;
            return;
        }
        else if (port != -1 && it->getPort() == port)
        {
            this->_server_config = *it;
            return;
        }
    }
    this->_server_config = g_config.getServerConfigs().front();
}

void Request::headerParsing()
{
    while (this->_req.find("\r\n") != std::string::npos && this->_req.find("\r\n") != 0)
    {
        std::string line = this->_req.substr(0, this->_req.find("\r\n"));
        if (line.find(": ") == std::string::npos)
        {
            this->_return_code = 400;
            throw std::runtime_error("Invalid header line: " + line);
        }
        std::string key = line.substr(0, line.find(": "));
        std::string value = line.substr(line.find(": ") + 2);

        this->_headers.updateHeader(key, value);

        int index = this->_headers.getIndex(key);
        switch (index)
        {
            case 9: // Host
            {
                findHost(value);
                break;
            }
            case 1: // Content-Length
            {
                if (value.find_first_not_of("0123456789") != std::string::npos)
                    this->_return_code = 400;
                else if (static_cast<size_t>(stringToLong(value)) > MAX_BODY_SIZE)
                    this->_return_code = 413;
                break;
            }
            case 2: // Transfer-Encoding
            {
                if (value != "chunked" || value != "identity")
                    this->_return_code = 400;
                break;
            }
            case 3: // Content-Type
            {
                if (value.find("text/") == std::string::npos && \
                    value.find("image/") == std::string::npos && \
                    value.find("audio/") == std::string::npos && \
                    value.find("video/") == std::string::npos && \
                    value.find("application/") == std::string::npos && \
                    value.find("multipart/") == std::string::npos)
                    this->_return_code = 415;
                break;
            }
            // Add the other headers case here
            default:
                break;
        }
        this->_req = this->_req.substr(this->_req.find("\r\n") + 2);
    }
}

void Request::bodyParsing()
{
    this -> _body = this->_req.substr(this->_req.find("\r\n") + 2);
    if (this->_body.length() < static_cast<size_t>(stringToLong(this->_headers.getHeader("Content-Length")))){
        this->_return_code = 400;
        throw std::runtime_error("Invalid body length");
    }
    if (this->_headers.getHeader("Transfer-Encoding") == "chunked")
    {
        std::string chunked = this->_body;
        std::string body = "";
        while (chunked.length() > 0)
        {
            size_t chunkSize = stringToLong(chunked.substr(0, chunked.find("\r\n")));
            if (chunkSize == 0)
                break;
            body += chunked.substr(chunked.find("\r\n") + 2, chunkSize);
            chunked = chunked.substr(chunked.find("\r\n") + 2 + chunkSize);
        }
        this->_body = body;
    } else {
        this->_body = this->_body.substr(0, stringToLong(this->_headers.getHeader("Content-Length")));
    }
    return;
}