#include "../includes/Request.hpp"

void Request::getResponse()
{
    std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 12\r\n\r\nHello World!";
    return response;
}

void Request::headerParsing()
{
    this->_map = std::map<std::string, std::string>();
    while (this->_req.find("\r\n") != std::string::npos)
    {
        std::string line = this->_req.substr(0, this->_req.find("\r\n"));
        if (line == "")
            break;
        if (line.find(": ") == std::string::npos)
            throw std::runtime_error("Invalid header line: " + line);
        std::string key = line.substr(0, line.find(": "));
        std::string value = line.substr(line.find(": ") + 2);
        switch (key)
        {
            case ("Host"):
            {
                this->_host = value.substr(0, value.find(":"));
                if (value.find(":") == std::string::npos){
                    for (el in g_config.getServerConfigs()){
                        if (el.getServerName() == this->_host){
                            this->_port = el.getPort();
                            break;
                        }
                    }
                }
                else
                    this->_port = std::stoi(value.substr(value.find(":") + 1));
                break;
            }
            default:
                throw std::runtime_error("Invalid header key: " + key);
        }
    }
}

void Request::bodyParsing()
{
    return;
}