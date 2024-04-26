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
	std::cout <<" path : " << path << std::endl;
	for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++)
	{
		if (it->getPath() == path)
			return *it;
	}
	return Location();
}

void Request::locationParsing()
{
	// checking server config locations
	std::string location = "/";
	std::size_t slashPos = this->_path.find('/', location.size());

	std::cout << "path : " << this->_path << std::endl;
	std::cout << "location : " << location << std::endl;
	std::cout << "slashPos : " << slashPos << std::endl;
	std::cout << std::endl;

	while (slashPos != std::string::npos && isLocation(location + this->_path.substr(location.size(), slashPos - location.size()))){
		location += this->_path.substr(location.size(), slashPos - location.size());
		slashPos = this->_path.find('/', slashPos + 1);
		std::cout << "location : " << location << std::endl;
		std::cout << "slashPos : " << slashPos << std::endl;
	}

	this->_path.erase(0, location.size()); //
	this->_location = findLocation(location);
	if (this->_location.getPath() != location)
		this->_path = this->_server_config.getRoot() + this->_path;
	else
		this->_path = this->_location.getRoot() + this->_path;

	if (getResourceType() == "directory" || getResourceType() == "root"){
		if (this->_location.getIndex().empty())
			this->_file = "index.html";
		else
			this->_file = this->_location.getIndex();
		this->_path += "/" + this->_file;
	} else if (getResourceType() == "file"){
		this->_file = this->_path.substr(this->_path.find_last_of('/') + 1);
	} else {
		this->_return_code = 404;
		std::cout << "code : " << this->_return_code << std::endl;
		throw std::runtime_error("Resource not found: " + this->_path);
	}
}