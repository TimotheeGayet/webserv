#include "../../includes/Globals.hpp"
#include "../../includes/response/Response.hpp"

Response::Response(Request& request) : _request(request) {}

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

std::string Response::ErrorResponse()
{
	ServerConfig 	server = this->_request.getServerConfig();
	int 			err_code = this->_request.getReturnCode();
	std::string 	err_msg = g_config.getDefaultErrors().getError(err_code);
	std::string 	err_page = g_config.getDefaultErrors().getErrorPage(err_code); // Default error page
	Location		location = this->_request.getLocation();

	if (location.getErrorPages().size() != 0 && location.getErrorPages().find(err_code) != location.getErrorPages().end())\
		err_page = location.getErrorPages().find(err_code)->second; // Custom error page

	// Composing the Error Response
	std::stringstream ss;
	ss << "HTTP/1.1 " << err_msg << "\r\n";
	ss << "Server: " << server.getServerName() << "\r\n";
	ss << "Date: " << getCurrentTime() << "\r\n";
	ss << "Connection: close\r\n";
	ss << "Content-Type: text/html\r\n";
	ss << "Content-Length: " << err_page.length() << "\r\n";
	ss << "\r\n";
	ss << err_page;
	
	return ss.str();
}

std::string Response::getResponse()
{
	std::string path = this->_request.getServerConfig().getRoot() + this->_request.getPath();
	std::string filename = this->_request.getFile();
	std::string contentType = "text/html";
	std::string extension;

	if (!filename.empty()) {
		extension = filename.substr(filename.find_last_of('.'));
		if (extension == ".php" || extension == ".html") {
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
		} else if (extension == ".txt") {
			contentType = "text/plain";
		}
	}

	std::ifstream file(path.c_str());
	if (!file.is_open() || this->_request.getReturnCode() != 200)
		return ErrorResponse();

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