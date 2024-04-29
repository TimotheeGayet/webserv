#include "../../includes/request/Request.hpp"

Location Request::getLocation() const {
	return this->_location;
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

Location Request::findLocation(const std::string& path) {
	std::vector<Location> locations = this->_server_config.getLocations();
	for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++)
	{
		if (it->getPath() == path)
			return *it;
	}
	return Location();
}


void Request::locationParsing()
{
	std::string location = this->_path;
	
	while (isLocation(location) == false) {
		location = location.substr(0, location.find_last_of('/'));
		if (location.empty())
			location = "/";
		if (location == "/")
			break;
	}

	if (isLocation(location) == false) {
		this->_return_code = 404;
		throw std::runtime_error("Resource not found: " + this->_path);
	};

	this->_location = findLocation(location);

	if (location == this->_path) {
		if (this->_location.getRoot().empty()) {
			this->_path = this->_server_config.getRoot();
		} else {
			this->_path = this->_location.getRoot();
		}
	}
	else {
		if (this->_location.getRoot().empty()) {
			this->_path = this->_server_config.getRoot() + "/" + this->_path.substr(location.size());
		} else {
			this->_path = this->_location.getRoot() + "/" + this->_path.substr(location.size());
		}
	}
	std::string::size_type pos = 0;
	while ((pos = this->_path.find("//", pos)) != std::string::npos) {
		this->_path.replace(pos, 2, "/");
	}

	if (getResourceType() == "directory" || getResourceType() == "root"){
		if (this->_location.getIndex().empty())
			this->_file = "index.html";
		else
			this->_file = this->_location.getIndex();
		if (this->_path[this->_path.size() - 1] != '/')
			this->_path += "/";
		this->_path += this->_file;
	} else if (getResourceType() == "file") {
		this->_file = this->_path.substr(this->_path.find_last_of('/') + 1);
	} else {
		this->_return_code = 404;
		throw std::runtime_error("Resource not found: " + this->_path);
	}
}