#include "../../includes/Globals.hpp"
#include "../../includes/request/Request.hpp"
#include "../../includes/header/Header.hpp"

HeaderRequest Request::headerParsing()
{
    HeaderRequest header;

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

        if (key == "Host")
            header.handleHost(value, this->_server_config);
        // else if (key == "Accept")
        //     header.setAccept(value);
        // else if (key == "Connection")
        //     header.setConnection(value);
        else if (this->_method == "POST" || key == "Transfer-Encoding")
            HeaderRequest::handleTransferEncoding(value, this->_return_code, this->_req, this->_body);
        else if (this->_method == "POST" || key == "Content-Type")
            header.handleContentType(value, this->_return_code);
        else if (this->_method == "POST" || key == "Content-Length")
            header.handleContentLength(value, this->_return_code);
        // else
        // {
        //     this->_return_code = 400;
        //     throw std::runtime_error("Invalid header key: " + key);
        // }

        this->_req = this->_req.substr(this->_req.find("\r\n") + 2);
    }
    
    return header;
}