#include "../includes/Globals.hpp"
#include "../includes/Request.hpp"
#include "../includes/config/ServerConfig.hpp"
#include <algorithm>

std::string Request::getResponse()
{
    std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 12\r\n\r\nHello World!";
    return response;
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
    this->_headers = std::map<std::string, std::string>();
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
        std::string headers_list[] = {"Host", "Content-Length", "Transfer-Encoding"}; // Add other headers here
        int headers_list_size = sizeof(headers_list) / sizeof(headers_list[0]);
        int index = -1;
        for (int i = 0; i < headers_list_size; ++i)
        {
            if (headers_list[i] == key)
            {
                index = i;
                break;
            }
        }
        switch (index)
        {
            case 0: // Host
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
                else
                    this->_content_length = stringToLong(value);
                break;
            }
            case 2: // Transfer-Encoding
            {
                if (value == "chunked" || value == "identity")
                    this->_transfer_encoding = stringToLong(value);
                else
                    this->_return_code = 400;
                break;
            }
            // Add the other headers case here
            default:
                std::cout << "Header not found: " << value << std::endl;
        }
    }
}

void Request::bodyParsing()
{
    this -> _body = this->_req.substr(this->_req.find("\r\n\r\n") + 4);
    if (this->_body.length() != this->_content_length)
        this->_return_code = 400;
    if (this->_transfer_encoding == "chunked")
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
        this->_body = this->_body.substr(0, this->_content_length);
    }
    return;
}