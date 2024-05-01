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

	// If the location or the server has a custom error page, use it (location has priority)
	if (server.getErrorPages().size() != 0 && server.getErrorPages().find(err_code) != server.getErrorPages().end())
	{
		err_page = server.getErrorPages().find(err_code)->second;
	}
	if (location.getErrorPages().size() != 0 && location.getErrorPages().find(err_code) != location.getErrorPages().end())
	{
		err_page = location.getErrorPages().find(err_code)->second;
	}

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

std::string Response::Redirect(HeaderRequest& header) {
	std::stringstream ss;
	ss << "HTTP/1.1 301 Moved Permanently\r\n";
	ss << "Server: serveur_du_web\r\n";
	ss << "Location: " << this->_request.getLocation().getRedirectUrl() << "\r\n";
	ss << "Date: " << getCurrentTime() << "\r\n";
	ss << "Connection: " << header.getConnection() << "\r\n";
	ss << "\r\n";
	return ss.str();
}

std::string detectContentType(const std::string& filename) {
	std::string extension;
	if (filename.find_last_of(".") != std::string::npos)
		extension = filename.substr(filename.find_last_of("."));
	if (extension == ".php") {
		return "text/html";
	} else if (extension == ".html") {
		return "text/html";
	} else if (extension == ".css") {
		return "text/css";
	} else if (extension == ".jpg" || extension == ".jpeg") {
		return "image/jpeg";
	} else if (extension == ".png") {
		return "image/png";
	} else if (extension == ".gif") {
		return "image/gif";
	} else if (extension == ".ico") {
		return "image/x-icon";
	} else {
		return "text/plain";
	}
}

std::string Response::getResponse()
{
	HeaderRequest header = this->_request.getHeader();
	std::string path = this->_request.getServerConfig().getRoot() + this->_request.getPath();
	std::string contentType = detectContentType(this->_request.getPath());
	std::string code = "200 OK";
	std::vector<AcceptElement> accept = header.getAccept();

	// Redirection :
	if (this->_request.getDoRedirect() == true) {
		return Redirect(header);
	}

	if (this->_request.getReturnCode() != 200) {
		return ErrorResponse(this->_request.getReturnCode());
	}

	if (!isContentTypeAccepted(accept, contentType)) {
		return ErrorResponse(406);
	} 

	std::ifstream file(path.c_str());
	if (!file.is_open())
	{
		if (this->_request.getLocation().getRedirectUrl() != "")
			return Redirect(header);
		else if (this->_request.getLocation().getAutoindex())
			this->_response = generate_listing_html(path);
		else
			return ErrorResponse(404);
	}
	else {
		std::string line;
		while (std::getline(file, line) && !file.eof())
			this->_response += line + "\n";
	}

	if (this->_request.getMethod() == "POST")
		code = "201 Created";

	std::stringstream ss;
	ss << "HTTP/1.1 " << code << "\r\n";
	ss << "Server: serveur_du_web\r\n";
	ss << "Date: " << getCurrentTime() << "\r\n";
	ss << "Connection: " << header.getConnection() << "\r\n";
	ss << "Content-Type: " << contentType << "\r\n";
	ss << "Content-Length: " << this->_response.length() << "\r\n";
	ss << "\r\n";
	ss << this->_response;

	return ss.str();
}