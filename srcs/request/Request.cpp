#include "../../includes/request/Request.hpp"
#include "../../includes/Globals.hpp"
#include <string>

void checkMethod(const std::string method, int &return_code, std::vector<std::string> allowed_methods)
{
	if (allowed_methods.empty())
		return;

	for (std::vector<std::string>::iterator it = allowed_methods.begin(); it != allowed_methods.end(); it++)
	{
		if (*it == method)
			return;
	}
	return_code = 405;
	throw std::runtime_error("Method Not Allowed by location: " + method);
}

Request::Request(const std::string& msg) : _req(msg), _return_code(200), _port(0), _do_redirect(false) {
	try
	{
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
		if (this->_method != "GET" && this->_method != "POST" && this->_method != "DELETE" && this->_method != "PUT")
		{
			this->_return_code = 405;
			throw std::runtime_error("Method Not Allowed: " + this->_method);
		}

		isValidURI();
		this->_header = headerParsing();
		locationParsing();
		checkMethod(this->_method, this->_return_code, this->_location.getAllowedMethods());
		bodyParsing();

		if (this->_method == "GET")
			getFileContent();
		else if (this->_method == "POST" || this->_method == "PUT")
			uploadFile();
		else if (this->_method == "DELETE")
			deleteFile();

		_req = msg;
	}
	catch (std::exception &e)
	{
		std::cerr << "[ERROR]: webserv: " << e.what() << std::endl;
	}
}

Request::~Request() {}

HeaderRequest Request::getHeader() const {
	return this->_header;
}

bool Request::getDoRedirect() const {
	return this->_do_redirect;
}

std::string Request::getRequest() {
	return this->_req;
}

std::string Request::getURI() const {
	return this->_uri;
}

void Request::setDoRedirect(bool do_redirect) {
	this->_do_redirect = do_redirect;
}