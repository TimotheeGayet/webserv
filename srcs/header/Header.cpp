#include "../../includes/header/Header.hpp"

// Constructors and destructors ------------------------------------------------

HeaderRequest::HeaderRequest() {}
HeaderRequest::~HeaderRequest() {}

HeaderResponse::HeaderResponse() {}
HeaderResponse::~HeaderResponse() {}


// Request getters and setters -------------------------------------------------

std::string HeaderRequest::getHost() const { return this->_host; }
std::string HeaderRequest::getAccept() const { return this->_accept; }
std::string HeaderRequest::getUserAgent() const { return this->_user_agent; }
std::string HeaderRequest::getConnection() const { return this->_connection; }
std::string HeaderRequest::getContentType() const { return this->_content_type; }
std::string HeaderRequest::getContentLength() const { return this->_content_length; }

void HeaderRequest::setHost(const std::string& host)
{
    this->_host = host;
}

void HeaderRequest::setAccept(const std::string& accept)
{
    this->_accept = accept;
}

void HeaderRequest::setUserAgent(const std::string& user_agent)
{
    this->_user_agent = user_agent;
}

void HeaderRequest::setConnection(const std::string& connection)
{
    this->_connection = connection;
}

void HeaderRequest::setContentType(const std::string& content_type)
{
    this->_content_type = content_type;
}

void HeaderRequest::setContentLength(const std::string& content_length)
{
    this->_content_length = content_length;
}


// Response getters and setters ------------------------------------------------

std::string HeaderResponse::getDate() const { return this->_date; }
std::string HeaderResponse::getServer() const { return this->_server; }
std::string HeaderResponse::getConnection() const { return this->_connection; }
std::string HeaderResponse::getContentType() const { return this->_content_type; }
std::string HeaderResponse::getContentLength() const { return this->_content_length; }

void HeaderResponse::setServer(const std::string& server)
{
    this->_server = server;
}

void HeaderResponse::setDate(const std::string& date)
{
    this->_date = date;
}

void HeaderResponse::setContentType(const std::string& content_type)
{
    this->_content_type = content_type;
}

void HeaderResponse::setContentLength(const std::string& content_length)
{
    this->_content_length = content_length;
}

void HeaderResponse::setConnection(const std::string& connection)
{
    this->_connection = connection;
}
