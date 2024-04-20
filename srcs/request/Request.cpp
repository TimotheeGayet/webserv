#include "../../includes/request/Request.hpp"
#include "../../includes/Globals.hpp"
#include <string>

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

Request::Request(const std::string& msg) : _return_code(200), _req(msg), _method(""), _uri(""), _host(""), _port(80), _path(""), _query(""), _fragment(""), _version(""), _body(""), _headers()
{
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

		if (count != 3 || this->_version != "HTTP/1.1\r" || \
			line.substr(line.length() - 1) != "\r" || (this->_method != "GET" && this->_method != "POST" && this->_method != "DELETE"))
		{
			this->_return_code = 400;
			throw std::runtime_error("Invalid request line: " + line);
		}
		isValidURI();
		headerParsing();
		locationParsing();
		bodyParsing();
	}
	catch (std::exception &e)
	{
		std::cerr << "webserv: Error: " << e.what() << std::endl << std::endl;
		std::stringstream ss;
		ss << "HTTP/1.1 " << g_config.getDefaultErrors().getError(this->_return_code) << "\r\n";
		ss << "Server: serveur_du_web\r\n";
		ss << "Date: " << getCurrentTime() << "\r\n";
		ss << "Content-Length:" << g_config.getDefaultErrors().getErrorPage(this->_return_code).length() << "\r\n";
		ss << "Connection: close\r\n";
		ss << "\r\n";
		ss << g_config.getDefaultErrors().getErrorPage(this->_return_code) << "\r\n";
		this->_response = ss.str();
	}
}

Request::~Request() {}

std::string Request::getFile()
{
	return this->_file;
}


std::string Request::getResponse()
{
	std::string path = this->_server_config.getRoot() + this->_path;
	std::string filename = this->_file;
	std::string contentType = "text/html"; // default to HTML

	if (!filename.empty()) {
		std::string extension = filename.substr(filename.find_last_of('.'));
		if (extension == ".php") {
			this->_response = CgiHandler::execute_cgi(path);
			contentType = "text/html"; // PHP scripts output HTML
		} else if (extension == ".css") {
			contentType = "text/css"; // CSS files
		} else if (extension == ".jpg" || extension == ".jpeg") {
			contentType = "image/jpeg"; // JPEG images
		} else if (extension == ".png") {
			contentType = "image/png"; // PNG images
		} else if (extension == ".js") {
			contentType = "application/javascript"; // JavaScript files
		}
	}

	std::ifstream file(path.c_str());
	if (!file.is_open() || !file.good() || this->_return_code == 400)
		return "HTTP/1.1 404 Not Found\r\nServer: serveur_du_web\r\nDate: " + getCurrentTime() + "\r\nContent-Length: 253\r\nConnection: close\r\n\r\n" + g_config.getDefaultErrors().get404() + "\r\n";

	std::string line;
	while (std::getline(file, line) && !file.eof())
		this->_response += line + "\n";

	std::stringstream ss;
	ss << "HTTP/1.1 200 OK\r\n";
	ss << "Server: serveur_du_web\r\n";
	ss << "Date: " << getCurrentTime() << "\r\n";
	ss << "Content-Type: " << contentType << "; charset=UTF-8\r\n"; // set the Content-Type header
	ss << "Content-Length: " << this->_response.length() << "\r\n";
	ss << "Connection: keep-alive\r\n";
	ss << "\r\n";
	ss << this->_response;
	return ss.str();
}

bool Request::isLocation(const std::string& path) {
	std::vector<Location> locations = this->_server_config.getLocations();
	for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++)
	{
		if (it->getPath() == path){
			return true;
		}
	}
	return false;
}

Location Request::getLocation(const std::string& path) {
	std::vector<Location> locations = this->_server_config.getLocations();
	for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++)
	{
		if (it->getPath() == path)
			return *it;
	}
	return Location();
}

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

void Request::locationParsing()
{
	// checking server config locations
	std::string location = "/";
	std::size_t slashPos = this->_path.find('/', location.size());
	while (slashPos != std::string::npos && isLocation(location + this->_path.substr(location.size(), slashPos - location.size()))){
		location += this->_path.substr(location.size(), slashPos - location.size());
		slashPos = this->_path.find('/', slashPos + 1);
	}

	this->_path.erase(0, location.size());
	Location loc = getLocation(location);
	if (loc.getPath() != location)
		this->_path = this->_server_config.getRoot() + this->_path;
	else
		this->_path = loc.getRoot() + this->_path;

	if (getResourceType() == "directory" || getResourceType() == "root"){
		if (loc.getIndex().empty()){
			this->_file = "index.html";
		}
		else
			this->_file = loc.getIndex();
		this->_path += "/" + this->_file;
	} else if (getResourceType() == "file"){
		this->_file = this->_path.substr(this->_path.find_last_of('/') + 1);
	} else {
		this->_return_code = 404;
		throw std::runtime_error("Resource not found: " + this->_path);
	}
}

std::string Request::getPath() {
	return this->_path;
}

ServerConfig Request::getServerConfig() {
	return this->_server_config;
}