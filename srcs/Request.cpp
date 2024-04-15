#include "../includes/Request.hpp"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

Request::Request(size_t client_fd, const std::string& msg) : _req(msg), _method("GET"), _uri("http://localhost:80"), _host("localhost"), _port(80), _path(""), _query(""), _fragment(""), _version("1.1"), _body("")
{
	(void)client_fd;

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

		if (count != 3)
			throw std::runtime_error("Invalid request line: " + line);
		if (line.substr(line.length() - 1) != "\r")
			throw std::runtime_error("Header line does not end with \\r\\n : " + line);

		if (this->_method != "GET" && this->_method != "POST" && this->_method != "DELETE")
			throw std::runtime_error("Invalid method: " + this->_method);
		isValidURI();
		if (this->_version != "HTTP/1.1\r")
			throw std::runtime_error("Invalid version: " + this->_version);
		// Add the parameter parsing
		// Add the body parsing
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
	std::string authority;
	std::size_t index = 0;

	if (this->_uri.empty() ||
		this->_uri.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-._~:/?#[]@!$&'()*+,;=%") != std::string::npos)
		throw std::runtime_error("Invalid URI: " + this->_uri);

	if (this->_uri.substr(index, 5) == "http:")
		index = 5;

	// Authority parsing
	std::size_t authorityStart = this->_uri.find("//");
	if (authorityStart != std::string::npos)
	{
		std::size_t authorityEnd = this->_uri.find_first_of("/?#", authorityStart + 2);
		if (authorityEnd == std::string::npos)
			authorityEnd = this->_uri.length();
		authority = this->_uri.substr(authorityStart + 2, authorityEnd - authorityStart - 2);
		std::size_t portStart = authority.find(':');
		if (portStart != std::string::npos && portStart != authority.length() - 1)
		{
			std::string port = authority.substr(portStart + 1);
			if (port.find_first_not_of("0123456789") != std::string::npos)
				throw std::runtime_error("Invalid port: " + this->_uri);
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
