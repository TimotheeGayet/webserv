#include "../../includes/Globals.hpp"
#include "../../includes/request/Request.hpp"

typedef void (Request::*HandlerFunction)(const std::string&);

std::map<std::string, HandlerFunction> handlers;

void Request::handleHost(const std::string& value)
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

void Request::handleContentLength(const std::string& value) {
    if (value.find_first_not_of("0123456789") != std::string::npos)
        this->_return_code = 400;
    else if (static_cast<size_t>(stringToLong(value)) > MAX_BODY_SIZE)
        this->_return_code = 413;
}

void Request::handleTransferEncoding(const std::string& value) {
    if (value != "chunked" || value != "identity")
        this->_return_code = 400;
}

void Request::handleContentType(const std::string& value) {
    if (value.find("text/") == std::string::npos && \
        value.find("image/") == std::string::npos && \
        value.find("audio/") == std::string::npos && \
        value.find("video/") == std::string::npos && \
        value.find("application/") == std::string::npos && \
        value.find("multipart/") == std::string::npos)
            this->_return_code = 415;
    // To complete
}

void Request::setupHandlers() {
    handlers["Host"] = &Request::handleHost;
    handlers["Content-Length"] = &Request::handleContentLength;
    handlers["Transfer-Encoding"] = &Request::handleTransferEncoding;
    handlers["Content-Type"] = &Request::handleContentType;
}

void Request::processHeaders(const std::map<std::string, std::string>& headers) {
    setupHandlers();
    std::map<std::string, std::string>::const_iterator it = headers.begin();
    std::map<std::string, std::string>::const_iterator end = headers.end();
    for (; it != end; ++it) {
        std::map<std::string, HandlerFunction>::const_iterator handler = handlers.find(it->first);
        if (handler != handlers.end()){
            (this->*handler->second)(it->second);
        }
    }
}
