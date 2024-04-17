#include "../includes/Request.hpp"
#include "../includes/Globals.hpp"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

Request::Request(const std::string& msg) : _return_code(200), _req(msg), _method(""), _uri(""), _host(""), _port(80), _path(""), _query(""), _fragment(""), _version(""), _body(""), _headers(), _content_length(0), _transfer_encoding("")
{

	std::cout << "Request: " << msg << std::endl;

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

void Request::Answer()
{
	// Temporary function to test the parsing
	std::cout << "Method: " << this->_method << std::endl;
	std::cout << "URI: " << this->_uri << std::endl;
	std::cout << "Version: " << this->_version << std::endl;
}

void Request::isValidURI()
{
	std::size_t index = 0;

	if (this->_uri.empty() ||
		this->_uri.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-._~:/?#[]@!$&'()*+,;=%") != std::string::npos){
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
}
