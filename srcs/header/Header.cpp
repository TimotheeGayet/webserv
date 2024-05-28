#include "../../includes/header/Header.hpp"

HeaderRequest::HeaderRequest() : _connection("close"), _content_length(0), _transfer_encoding("identity") {}

HeaderRequest::~HeaderRequest() {}

AcceptElement parseAcceptElement(const std::string& element) {
    AcceptElement result;
    size_t qIndex = element.find(";q=");
    
    if (qIndex != std::string::npos) {
        result.typeMIME = element.substr(0, qIndex);
        std::string qualityStr = element.substr(qIndex + 3);
        result.quality = static_cast<float>(atof(qualityStr.c_str()));
    } else {
        result.typeMIME = element;
        result.quality = 1.0f;
    }

    return result;
}

void HeaderRequest::handleHost(const std::string& value, ServerConfig& server_config, u_int16_t port)
{
    size_t colonPos = value.find(':');
    std::string host = value.substr(0, colonPos);
    if (colonPos != std::string::npos)
        port = std::strtol(value.substr(colonPos + 1).c_str(), NULL, 10);

    const std::vector<ServerConfig>& serverConfigs = g_config.getServerConfigs();
    std::vector<ServerConfig>::const_iterator it = serverConfigs.begin();
    std::vector<ServerConfig>::const_iterator end = serverConfigs.end();
    for (; it != end; ++it)
    {
        if (it->getServerName() == host && (it->getPort() == port || port == 0))
        {
            server_config = *it;
            return;
        }
    }
    server_config = g_config.getServerConfigs().front();
}

void HeaderRequest::handleAccept(const std::string& value) {
    std::istringstream iss(value);
    std::string element;
    
    while (std::getline(iss, element, ',')) {
        size_t start = element.find_first_not_of(' ');
        size_t end = element.find_last_not_of(' ');
        if (start != std::string::npos && end != std::string::npos) {
            element = element.substr(start, end - start + 1);
        }
        this->_accept.push_back(parseAcceptElement(element));
    }
}


void HeaderRequest::handleConnection(const std::string& value) {
    if (value != "keep-alive" && value != "close") {
        throw std::runtime_error("Invalid Connection: " + value);
    }
    this->_connection = value;
}

void HeaderRequest::handleContentLength(const std::string& value, int& return_code)
{
    if (value.find_first_not_of("0123456789") != std::string::npos)
    {
        return_code = 400;
        throw std::runtime_error("Invalid Content-Length: " + value);
    }
    else if (value == "0")
    {
        return_code = 411;
        throw std::runtime_error("Content-Length is 0");
    }

    this->_content_length = std::strtol(value.c_str(), NULL, 10);
}

void HeaderRequest::handleTransferEncoding(const std::string& value, int& return_code) {
    if (value != "chunked" && value != "identity") {
        return_code = 400;
        throw std::runtime_error("Invalid Transfer-Encoding: " + value);
    }

    this->_transfer_encoding = value;
}

int HeaderRequest::getContentLength() const {
    return this->_content_length;
}

std::string HeaderRequest::getConnection() const {
    return this->_connection;
}

std::string HeaderRequest::getTransferEncoding() const {
    return this->_transfer_encoding;
}

std::vector<AcceptElement> HeaderRequest::getAccept() const {
    return this->_accept;
}