#include "../../includes/request/Request.hpp"
#include "../../includes/Globals.hpp"
#include <string>

Request::Request(const std::string& msg) : _req(msg), _return_code(200), _do_redirect(false) {

	try {
		std::stringstream ss(msg);
		std::string first_line;
		std::getline(ss, first_line);

		std::stringstream first_line_ss(first_line);
		std::string item;
		int count = 0;
		while (std::getline(first_line_ss, item, ' '))
		{
			count++;
			if (count == 1)
				this->_method = item;
			else if (count == 2)
				this->_uri = item;
			else if (count == 3)
				this->_version = item;
		}

		if (count != 3 || first_line.substr(first_line.length() - 1) != "\r")
		{
			this->_return_code = 400;
			throw std::runtime_error("Invalid request line: " + first_line);
		}
		if (this->_version != "HTTP/1.1\r")
		{
			this->_return_code = 505;
			throw std::runtime_error("HTTP Version Not Supported: " + first_line);
		}
		if (this->_method != "GET" && this->_method != "POST" && this->_method != "DELETE")
		{
			this->_return_code = 501;
			throw std::runtime_error("Not Implemented: " + first_line);
		}

		isValidURI();
		this->_header = headerParsing();
		locationParsing();
		bodyParsing();

		if (this->_method == "GET")
			getFile();
		else if (this->_method == "POST")
			uploadFile();
		else if (this->_method == "DELETE")
			deleteFile();
	}
	catch (std::exception &e)
	{
		std::cerr << "webserv: Error: " << e.what() << std::endl;
	}
}

Request::~Request() {}

HeaderRequest Request::getHeader() const {
	return this->_header;
}

bool Request::getDoRedirect() const {
	return this->_do_redirect;
}