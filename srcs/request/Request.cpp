#include "../../includes/request/Request.hpp"
#include "../../includes/Globals.hpp"
#include <string>

Request::Request(const std::string& msg) : _req(msg), _return_code(200){
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

		if (count != 3 || line.substr(line.length() - 1) != "\r")
		{
			this->_return_code = 400;
			throw std::runtime_error("Invalid request line: " + line);
		}
		if (this->_version != "HTTP/1.1\r")
		{
			this->_return_code = 505;
			throw std::runtime_error("HTTP Version Not Supported: " + line);
		}
		if (this->_method != "GET" && this->_method != "POST" && this->_method != "DELETE")
		{
			this->_return_code = 501;
			throw std::runtime_error("Not Implemented: " + line);
		}
		isValidURI();
		headerParsing();
		locationParsing();
		bodyParsing();
	}
	catch (std::exception &e)
	{
		std::cerr << "webserv: Error: " << e.what() << std::endl << std::endl;
	}
}

Request::~Request() {}