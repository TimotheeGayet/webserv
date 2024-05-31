#include "../../includes/request/Request.hpp"

bool Request::isLocation(const std::string& path)
{
	std::vector<Location> locations = this->_server_config.getLocations();
	for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++)
	{
		if (it->getPath() == path)
		{
			return true;
		}
	}
	return false;
}

Location Request::findLocation(const std::string& path)
{
	std::vector<Location> locations = this->_server_config.getLocations();
	for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++)
	{
		if (it->getPath() == path)
		{
			return *it;
		}
	}
	return Location();
}

void Request::resolveDirectoryPath()
{
    _file = _location.getIndex(); // Default index file in the location config

    if (_file.empty())
    {
        _file = "index.html"; // if no default index
    }

	_initial_path = _path;

    if (_path[_path.size() - 1] != '/')
    {
        _path += "/";
    }
	
    _path += _file;
}

void Request::resolvePath()
{
	_ressource_type = getResourceType(_path.c_str());

    if (_ressource_type == "directory" || _ressource_type == "root")
    {
        resolveDirectoryPath();
    }
    else if (_ressource_type == "file")
    {
        _file = _path.substr(_path.find_last_of('/') + 1); // Get the file name of the path
    }
    else if (_method != "POST")
    {
        _return_code = 404;

		if (_location.getRedirectUrl().empty() == false)
		{
			_do_redirect = true;
		}
    	throw std::runtime_error("Resource not found: " + _path);
    }
}

void Request::locationParsing()
{
	std::string asked_path = this->_path;
	std::string location = this->_path;

	while (isLocation(location) == false)
	{
		location = location.substr(0, location.find_last_of('/'));
		if (location.empty())
			location = "/";
		if (location == "/")
			break;
	}


	if (isLocation(location) == false)
	{
		this->_return_code = 404;
		throw std::runtime_error("Resource not found: " + this->_path);
	};

	this->_location = findLocation(location);

	if (asked_path == this->_location.getPath() && \
		this->_location.getRedirectUrl().empty() == false)
	{
		this->_do_redirect = true;
	}

	if (location == this->_path)
	{
		if (this->_location.getRoot().empty())
		{
			this->_path = this->_server_config.getRoot();
		}
		else
		{
			this->_path = this->_location.getRoot();
		}
	}
	else
	{
		if (this->_location.getRoot().empty())
		{
			this->_path = this->_server_config.getRoot() + "/" + this->_path.substr(location.size());
		}
		else
		{
			this->_path = this->_location.getRoot() + "/" + this->_path.substr(location.size());
		}
	}

	std::string::size_type pos = 0;
	while ((pos = this->_path.find("//", pos)) != std::string::npos) {
		this->_path.replace(pos, 2, "/");
	}

	resolvePath();
}