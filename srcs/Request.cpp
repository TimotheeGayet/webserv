#include "../includes/Request.hpp"
#include <iostream>
#include <sstream>

#include <iostream>
#include <string>
#include <algorithm>

Request::Request(std::string &msg)
{
	try
	{
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
		isValidURI(this->_uri);
		if (this->_version != "HTTP/1.1\r")
			throw std::runtime_error("Invalid version: " + this->_version);
		// Add all the parameters parsing
		// Add the body parsing
	}
	catch (std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
}

Request::~Request()
{
}

void Request::Answer()
{
	// Temporary function to test the parsing
	std::cout << "Method: " << this->_method << std::endl;
	std::cout << "URI: " << this->_uri << std::endl;
	std::cout << "Version: " << this->_version << std::endl;
}

bool Request::isValidURI(const std::string &uri)
{

	std::string authority;

	if (uri.empty() ||
		uri.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-._~:/?#[]@!$&'()*+,;=%") != std::string::npos)
		return false;

	// Scheme verification (maybe add a behavior to allow no-scheme URIs like NGINX in which it is http:// by default)
	if (uri.substr(0, 5) != "http:")
		return false;

	// Authority parsing
	std::size_t authorityStart = uri.find("//");
	if (authorityStart != std::string::npos)
	{
		std::size_t authorityEnd = uri.find_first_of("/?#", authorityStart + 2);
		authority = uri.substr(authorityStart + 2, authorityEnd - authorityStart - 2);
		std::size_t portStart = authority.find(':');
		if (portStart != std::string::npos && portStart != authority.length() - 1)
		{
			std::string port = authority.substr(portStart + 1);
			if (port.find_first_not_of("0123456789") != std::string::npos)
				return false;
		}
	}

	// Path parsing

	return true;
}
