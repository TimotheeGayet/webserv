#include "../../includes/request/Request.hpp"

unsigned long HexaStrToLong(const std::string& str)
{
    unsigned long result = 0;
    for (size_t i = 0; i < str.length(); i++)
    {
        if (str[i] >= '0' && str[i] <= '9')
            result = result * 16 + str[i] - '0';
        else if (str[i] >= 'a' && str[i] <= 'f')
            result = result * 16 + str[i] - 'a' + 10;
        else if (str[i] >= 'A' && str[i] <= 'F')
            result = result * 16 + str[i] - 'A' + 10;
        else
            return 0;
    }
    return result;
}

void Request::bodyParsing()
{
    this->_body = this->_req.substr(this->_req.find("\r\n") + 2);

    // Returns an error if there is a body with the GET/DELETE method
    if (this->_method == "GET" || this->_method == "DELETE")
    {
        if (this->_body.length() > 0)
        {
            this->_return_code = 400;
            throw std::runtime_error("GET/DELETE request should not have a body");
        }
        return;
    }

    // Content-Length checking
    size_t max_size = MAX_BODY_SIZE;
    if (this->_server_config.getClientMaxBodySize() != "0")
        max_size = stringToLong(this->_server_config.getClientMaxBodySize());
    if (this->_location.getClientMaxBodySize() != "0")
        max_size = stringToLong(this->_location.getClientMaxBodySize());

    if (this->_header.getContentLength() > 0)
    {
        if (this->_header.getContentLength() != static_cast<int>(this->_body.length()))
        {
            this->_return_code = 400;
            throw std::runtime_error("Invalid Content-Length");
        }
        else if (this->_header.getContentLength() > static_cast<int>(max_size))
        {
            this->_return_code = 413;
            throw std::runtime_error("Payload Too Large");
        }
    }
    else if (this->_header.getContentLength() < 0)
    {
        this->_return_code = 400;
        throw std::runtime_error("Invalid Content-Length");
    }

    // Transfer encoding checking
    std::string decoded_body = "";
    size_t pos = 0;
    size_t end_pos = 0;
    size_t chunk_size = 0;

    if (this->_body.size() == 0)
        return;

    if (this->_header.getTransferEncoding() == "chunked")
    {
        if (this->_body.find("\r\n") == std::string::npos)
        {
            this->_return_code = 400;
            throw std::runtime_error("Invalid chunked encoding");
        }
        else if (this->_header.getContentLength() != 0)
        {
            this->_return_code = 400;
            throw std::runtime_error("Content-Length must not be set with Transfer-Encoding");
        }
        while (pos < this->_body.length())
        {
            end_pos = this->_body.find("\r\n", pos);
            if (end_pos == std::string::npos)
            {
                this->_return_code = 400;
                throw std::runtime_error("Invalid chunked encoding");
            }

            chunk_size = HexaStrToLong(this->_body.substr(pos, end_pos - pos));

            if (chunk_size == 0)
                break;

            if (end_pos + 2 + chunk_size > this->_body.length() || this->_body.substr(end_pos + 2 + chunk_size, 2) != "\r\n")
            {
                this->_return_code = 400;
                throw std::runtime_error("Invalid chunked encoding");
            }

            decoded_body += this->_body.substr(end_pos + 2, chunk_size);

            if (decoded_body.length() > max_size)
            {
                this->_return_code = 413;
                throw std::runtime_error("Payload Too Large");
            }

            pos = end_pos + 2 + chunk_size + 2;
        }

        this->_body = decoded_body;
    }
}