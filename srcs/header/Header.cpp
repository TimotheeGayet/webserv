#include "../../includes/header/Header.hpp"

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


static long stringToLong(const std::string& str) {
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

HeaderRequest::HeaderRequest() : _content_length(0) {}

HeaderRequest::~HeaderRequest() {}

void HeaderRequest::handleHost(const std::string& value, ServerConfig& server_config)
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
            server_config = *it;
            return;
        }
        else if (port != -1 && it->getPort() == port)
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
    // for (std::vector<AcceptElement>::iterator it = this->_accept.begin(); it != this->_accept.end(); ++it) {
    //     std::cout << "Type MIME: " << it->typeMIME << ", Qualité: " << it->quality << std::endl;
    // }
}


void HeaderRequest::handleConnection(const std::string& value) {
    if (value != "keep-alive" && value != "close") {
        throw std::runtime_error("Invalid Connection: " + value);
    }
    this->_connection = value;
}

void HeaderRequest::handleContentLength(const std::string& value, int& return_code) {
    if (value.find_first_not_of("0123456789") != std::string::npos) {
        return_code = 400;
        throw std::runtime_error("Invalid Content-Length: " + value);
    }
    else if (static_cast<size_t>(stringToLong(value)) > MAX_BODY_SIZE) {
        return_code = 413;
        throw std::runtime_error("Payload Too Large: " + value);
    }
}

void HeaderRequest::handleTransferEncoding(const std::string& value, int& return_code) {
    if (value != "chunked" || value != "identity") {
        return_code = 400;
        throw std::runtime_error("Invalid Transfer-Encoding: " + value);
    }
}

void HeaderRequest::handleContentType(const std::string& value, int& return_code) {
    if (value.find("text/") == std::string::npos && \
        value.find("image/") == std::string::npos && \
        value.find("audio/") == std::string::npos && \
        value.find("video/") == std::string::npos && \
        value.find("application/") == std::string::npos && \
        value.find("multipart/") == std::string::npos) {
            return_code = 415;
            throw std::runtime_error("Unsupported Media Type: " + value);
        }
    // To complete
}


int HeaderRequest::getContentLength() const {
    return this->_content_length;
}

std::string HeaderRequest::getConnection() const {
    return this->_connection;
}

std::string HeaderRequest::getContentType() const {
    return this->_content_type;
}

std::string HeaderRequest::getTransferEncoding() const {
    return this->_transfer_encoding;
}

std::vector<AcceptElement> HeaderRequest::getAccept() const {
    return this->_accept;
}