#include "../../includes/request/Request.hpp"
#include "../../includes/Globals.hpp"

Request::Request(const std::string& msg) : _return_code(200), _req(msg), _method(""), _uri(""), _host(""), _port(80), _path(""), _query(""), _fragment(""), _version(""), _body(""), _headers()
{
	try {
		std::stringstream ss(msg);
		std::string line;
		std::getline(ss, line);

		std::stringstream liness(line);
		std::string item;
		int count = 0;
		while (std::getline(liness, item, ' '))
		{
			count++;
			if (count == 1)
				this->_method = item;
			else if (count == 2)
				this->_uri = item;
			else if (count == 3)
				this->_version = item;
		}

		if (count != 3 || \
			this->_version != "HTTP/1.1\r" || \
			line.substr(line.length() - 1) != "\r" || \
			(this->_method != "GET" && this->_method != "POST" && this->_method != "DELETE")){
			this->_return_code = 400;
			throw std::runtime_error("Invalid request line: " + line);
		}
		isValidURI();
		headerParsing();
		bodyParsing();
	}
	catch (std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
}

Request::~Request() {}

std::string getCurrentTime()
{
	time_t rawtime;
	struct tm *timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, 80, "%a, %d %b %Y %H:%M:%S GMT", timeinfo);
	return std::string(buffer);
}

std::string Request::getResponse()
{
	std::string path = this->_server_config.getRoot() + this->_path;
	std::ifstream file(path.c_str());
	if (!file.is_open())
	{
		this->_return_code = 404;
		throw std::runtime_error("Resource not found: " + this->_path);
	}

	std::string line;
	while (std::getline(file, line)	&& !file.eof())
		this->_response += line + "\n";

	std::string response = "HTTP/1.1 200 OK\r\n";
	response += "Server: webserv/1.0\r\n";
	response += "Date: " + getCurrentTime() + "\r\n";
	response += "Content-Type: text/plain \r\n";
	response += "Content-Length: ";
	response += static_cast<std::ostringstream*>( &(std::ostringstream() << this->_response.length()) )->str();
	response += "\r\n";
	response += "Connection: close\r\n";
	response += "\r\n";
	response += this->_body;
	return response;
}

void Request::isValidURI()
{
	std::size_t index = 0;

	if (this->_uri.empty() || \
		this->_uri.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-._~:/?#[]@!$&'()*+,;=%") != std::string::npos || \
		this->_uri.size() > MAX_URI_SIZE){
			this->_return_code = 400;
			throw std::runtime_error("Invalid URI: " + this->_uri);
		}

	// Scheme parsing
	if (this->_uri.substr(index, 5) == "http:")
		index = 5;

	// Authority parsing
	std::size_t authorityStart = this->_uri.find("//");
	if (authorityStart != std::string::npos)
	{
		std::size_t authorityEnd = this->_uri.find_first_of("/?#", authorityStart + 2);
		if (authorityEnd == std::string::npos)
			authorityEnd = this->_uri.length();
		this->_host = this->_uri.substr(authorityStart + 2, authorityEnd - authorityStart - 2);
		std::size_t portStart = this->_host.find(':');
		if (portStart != std::string::npos && portStart != this->_host.length() - 1)
		{
			std::string port = this->_host.substr(portStart + 1);
			if (port.find_first_not_of("0123456789") != std::string::npos){
				this->_return_code = 400;
				throw std::runtime_error("Invalid port: " + this->_uri);
			}
			this->_port = std::strtod(port.c_str(), NULL);
		}
		index = authorityEnd;
	}

	// Path parsing
	std::size_t pathStart = this->_uri.find('/', index);
	if (pathStart != std::string::npos)
	{
		std::size_t queryStart = this->_uri.find('?', pathStart + 1);
		std::size_t fragmentStart = this->_uri.find('#', pathStart + 1);
		if (queryStart != std::string::npos)
		{
			this->_path = this->_uri.substr(pathStart, queryStart - pathStart);
			if (fragmentStart != std::string::npos)
			{
				this->_query = this->_uri.substr(queryStart, fragmentStart - queryStart);
				this->_fragment = this->_uri.substr(fragmentStart);
			}
			else
				this->_query = this->_uri.substr(queryStart);
		}
		else if (fragmentStart != std::string::npos)
		{
			this->_path = this->_uri.substr(pathStart, fragmentStart - pathStart);
			this->_fragment = this->_uri.substr(fragmentStart);
		}
		else
			this->_path = this->_uri.substr(pathStart);
	}
	else
		this->_path = "/";

	if (this->_path.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-._~:/?#[]@!$&'()*+,;=%") != std::string::npos) {
		this->_return_code = 400;
		throw std::runtime_error("Invalid path: " + this->_path);
	}

	if (getResourceType() == "directory"){
		this->_file = "index.html";
		if (this->_path[this->_path.length() - 1] != '/')
			this->_path += "/";
		this->_path += this->_file;
	} else if (getResourceType() == "unknown") {
		this->_return_code = 404;
		throw std::runtime_error("Resource not found: " + this->_path);
	} else
		this->_file = this->_path.substr(this->_path.find_last_of('/') + 1);

	this->_req = this->_req.substr(this->_req.find("\r\n") + 2);
}
