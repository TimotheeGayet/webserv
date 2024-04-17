#include "../includes/Globals.hpp"
#include "../includes/Request.hpp"
#include "../includes/config/ServerConfig.hpp"

std::string Request::getResponse()
{
    std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 12\r\n\r\nHello World!";
    return response;
}

void Request::headerParsing()
{
    this->_headers = std::map<std::string, std::string>();
    while (this->_req.find("\r\n") != std::string::npos && this->_req.find("\r\n") != 0)
    {
        std::string line = this->_req.substr(0, this->_req.find("\r\n"));
        if (line.find(": ") == std::string::npos)
            throw std::runtime_error("Invalid header line: " + line);
        std::string key = line.substr(0, line.find(": "));
        std::string value = line.substr(line.find(": ") + 2);

        int headers_list_index[] = {0};
        std::string headers_list[] = {"Host"}; // Add other headers here

        switch (std::find(std::begin(headers_list), std::end(headers_list), key) - std::begin(headers_list))
        {
        case 0:
        {
            std::string::size_type colonPos = value.find(':');
            this->_host = value.substr(0, colonPos);
            if (colonPos == std::string::npos)
            {
                std::vector<ServerConfig>::const_iterator it = g_config.getServerConfigs().begin();
                std::vector<ServerConfig>::const_iterator end = g_config.getServerConfigs().end();
                for (; it != end; ++it)
                {
                    if (it->getServerName() == this->_host)
                    {
                        this->_port = it->getPort();
                        break;
                    }
                }
            }
            else
            {
                this->_port = std::stoi(value.substr(colonPos + 1));
            }
            break;
        }
        // Ajoutez ici les autres cas pour les autres clés d'en-tête
        default:
            throw std::runtime_error("Invalid header key: " + headerKey);
        }
    }

    void Request::bodyParsing()
    {
        return;
    }
