#include "../../includes/Globals.hpp"
#include "../../includes/request/Response.hpp"

Response::Response(Request& request) : _request(request), _headers() {}

Response::~Response() {}

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

std::string Response::getResponse() {
	std::string path = this->_request.getServerConfig().getRoot() + this->_request.getPath();
	std::string filename = this->_request.getFile();
	std::string contentType = "text/html";
	std::string extension;

	if (!filename.empty()) {
		extension = filename.substr(filename.find_last_of('.'));
		if (extension == ".php") {
			contentType = "text/html";
		} else if (extension == ".css") {
			contentType = "text/css";
		} else if (extension == ".jpg" || extension == ".jpeg") {
			contentType = "image/jpeg";
		} else if (extension == ".png") {
			contentType = "image/png";
		} else if (extension == ".gif") {
			contentType = "image/gif";
		} else if (extension == ".ico") {
			contentType = "image/x-icon";
		} else if (extension == ".html") {
			contentType = "text/html";
		} else if (extension == ".txt") {
			contentType = "text/plain";
		}
	}

	std::ifstream file(path.c_str());
	if (!file.is_open() || !file.good() || this->_request.getReturnCode() != 200)
	{
		std::stringstream ss;
		ss << "HTTP/1.1" << this->_request.getReturnCode() << " " << g_config.getDefaultErrors().getError(this->_request.getReturnCode()) << "\r\n";
		ss << "Server: serveur_du_web\r\nDate: " << getCurrentTime() << "\r\n";

		// Check if the Error pages have been redefined
		if (this->_request.getLocation().getErrorPages().find(this->_request.getReturnCode()) != this->_request.getLocation().getErrorPages().end())
		{
			ss << "Content-Length: " << this->_request.getLocation().getErrorPages().find(this->_request.getReturnCode())->second.length() << "\r\n";
			ss << "Connection: close\r\n\r\n";
			ss << this->_request.getLocation().getErrorPages().find(this->_request.getReturnCode())->second + "\r\n";
		}
		else if (this->_request.getServerConfig().getErrorPages().find(this->_request.getReturnCode()) != this->_request.getServerConfig().getErrorPages().end())
		{
			ss << "Content-Length: " << this->_request.getServerConfig().getErrorPages().find(this->_request.getReturnCode())->second.length() << "\r\n";
			ss << "Connection: close\r\n\r\n";
			ss << this->_request.getServerConfig().getErrorPages().find(this->_request.getReturnCode())->second + "\r\n";
		}
		else
		{
			ss << "Content-Length: " << g_config.getDefaultErrors().getErrorPage(this->_request.getReturnCode()).length() << "\r\n";
			ss << "Connection: close\r\n\r\n";
			ss << g_config.getDefaultErrors().getErrorPage(this->_request.getReturnCode()) + "\r\n";
		}
		return ss.str();
	}

	std::string line;
	while (std::getline(file, line) && !file.eof())
		this->_response += line + "\n";

	std::stringstream ss;
	ss << "HTTP/1.1 200 OK\r\n";
	ss << "Server: serveur_du_web\r\n";
	ss << "Date: " << getCurrentTime() << "\r\n";
	ss << "Connection: keep-alive\r\n";
	ss << "Content-Type: " << contentType << "\r\n";
	ss << "Content-Length: " << this->_response.length() << "\r\n";
	ss << "\r\n";
	ss << this->_response;
	return ss.str();
}