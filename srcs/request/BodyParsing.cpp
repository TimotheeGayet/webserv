#include "../../includes/request/Request.hpp"

void Request::bodyParsing()
{
    this -> _body = this->_req.substr(this->_req.find("\r\n") + 2);

    // Returns an error if there is a body with the GET/DELETE method
    if (this->_body.empty() && (this->_method == "GET" || this->_method == "DELETE")){
        this->_return_code = 400;
        throw std::runtime_error("Error: GET/DELETE method with body");
    } else if (this->_body.empty()){
        return;
    }
    
    // if (this->_headers.getHeader("Content-Length") == "")
    // {
    //     this->_return_code = 411;
    //     throw std::runtime_error("Content-Length header is missing");
    // }
    // if (this->_body.length() < static_cast<size_t>(this->stringToLong(this->_headers.getHeader("Content-Length"))))
    // {
    //     this->_return_code = 400;
    //     throw std::runtime_error("Invalid body length");
    // }
    // if (this->_headers.getHeader("Transfer-Encoding") == "chunked")
    // {
    //     std::string chunked = this->_body;
    //     std::string body = "";
    //     while (chunked.length() > 0)
    //     {
    //         size_t chunkSize = this->stringToLong(chunked.substr(0, chunked.find("\r\n")));
    //         if (chunkSize == 0)
    //             break;
    //         body += chunked.substr(chunked.find("\r\n") + 2, chunkSize);
    //         chunked = chunked.substr(chunked.find("\r\n") + 2 + chunkSize);
    //     }
    //     this->_body = body;
    // } else {
    //     this->_body = this->_body.substr(0, this->stringToLong(this->_headers.getHeader("Content-Length")));
    // }
    return;
}