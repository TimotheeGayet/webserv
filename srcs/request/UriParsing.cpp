#include "../../includes/request/Request.hpp"

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
			this->_port = std::strtoul(port.c_str(), NULL, 10);
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

	this->_req = this->_req.substr(this->_req.find("\r\n") + 2);
}