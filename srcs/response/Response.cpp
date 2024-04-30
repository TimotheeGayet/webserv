#include "../../includes/Globals.hpp"
#include "../../includes/cgi/CgiHandler.hpp"
#include "../../includes/response/Response.hpp"

Response::Response(Request& request) : _request(request), _status_code(200) {}

Response::~Response() {}

int Response::getStatusCode()
{
	return this->_status_code;
}

void Response::setStatusCode(int status_code)
{
	this->_status_code = status_code;
}

static bool isContentTypeAccepted(const std::vector<AcceptElement>& acceptElements, const std::string& contentType) {
	for (std::vector<AcceptElement>::const_iterator it = acceptElements.begin(); it != acceptElements.end(); ++it) {
		std::string type = it->typeMIME.substr(0, it->typeMIME.find('/'));
		std::string subtype = it->typeMIME.substr(it->typeMIME.find('/') + 1);

		if ((type == "*" && subtype == "*") || 
			(type == contentType.substr(0, contentType.find('/')) && (subtype == "*" || subtype == contentType.substr(contentType.find('/') + 1)))) {
				return true;
		}
	}
	return false;
}

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

std::string Response::ErrorResponse(int err_code)
{
	ServerConfig 	server = this->_request.getServerConfig();
	std::string 	err_msg = g_config.getDefaultErrors().getError(err_code);
	std::string 	err_page = g_config.getDefaultErrors().getErrorPage(err_code); // Default error page
	Location		location = this->_request.getLocation();

	// If the location has a custom error page, use it
	if (location.getErrorPages().size() != 0 && location.getErrorPages().find(err_code) != location.getErrorPages().end())\
		err_page = location.getErrorPages().find(err_code)->second;

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

	this->setStatusCode(err_code);
	return ss.str();
}

std::string Response::getResponse()
{
	if (this->_request.getReturnCode() != 200) {
		return ErrorResponse(this->_request.getReturnCode());
	}

	std::string path = this->_request.getServerConfig().getRoot() + this->_request.getPath();
	std::string filename = this->_request.getFile();
	std::string contentType = "text/html";
	std::string extension;
	HeaderRequest header = this->_request.getHeader();
	std::vector<AcceptElement> accept = header.getAccept();

	if (!filename.empty()) {
		extension = filename.substr(filename.find_last_of('.'));
		if (extension == ".php") {
			this->_response = CgiHandler::execute_cgi(path);
			contentType = "text/html";
		} else if (extension == ".html") {
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

	if (!isContentTypeAccepted(accept, contentType)) {
		return ErrorResponse(406);
	} 

	std::ifstream file(path.c_str());
	if (!file.is_open())
	{
		if (this->_request.getLocation().getAutoindex())
			this->_response = generate_listing_html(path);
		else
			return ErrorResponse(404);
	}
	else {
		std::string line;
		while (std::getline(file, line) && !file.eof())
			this->_response += line + "\n";
	}

	std::stringstream ss;
	ss << "HTTP/1.1 200 OK\r\n";
	ss << "Server: serveur_du_web\r\n";
	ss << "Date: " << getCurrentTime() << "\r\n";
	ss << "Connection: " << header.getConnection() << "\r\n";
	ss << "Content-Type: " << contentType << "\r\n";
	ss << "Content-Length: " << this->_response.length() << "\r\n";
	ss << "\r\n";
	ss << this->_response;

	return ss.str();
}