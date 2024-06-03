#include "../../includes/request/Request.hpp"

void Request::getFileContent()
{
	std::ifstream file(_path.c_str());
    std::string line;
    while (std::getline(file, line))
    {
        _response += line + "\n";
    }
}

void Request::uploadFile()
{
    if (_ressource_type == "directory")
    {
        _return_code = 400;
        throw std::runtime_error("Bad Request: cannot upload a directory");
    }

    std::string path_to_file = _path.substr(0, _path.find_last_of('/'));

    if (!_location.getClientBodyTempPath().empty())
    {
        path_to_file = _location.getClientBodyTempPath();
        if (path_to_file[path_to_file.size() - 1] != '/')
            path_to_file += "/";
        _path = path_to_file + _file;
    }

    // Check if the path to the file exists
    if (_ressource_type == "directory" || _ressource_type == "root")
    {
        _return_code = 404;
        throw std::runtime_error("Invalid path for file uploading: " + _path);
    }

    // Create the file first if it doesn't exists
    if (_ressource_type == "unknown")
    {
        std::ofstream new_file(_path.c_str(), std::ofstream::out);
        new_file.close();
    }

    std::ofstream file;
    file.open(_path.c_str(), std::ifstream::app);

    if (!file.is_open())
    {
        _return_code = 500;
        throw std::runtime_error("Internal Server Error: cannot open file for writing");
    }

    file << _body;
    file.close();
}

void Request::deleteFile()
{
    if (_ressource_type == "directory" || _ressource_type == "root")
    {
        _return_code = 400;
        throw std::runtime_error("Bad Request: cannot delete a directory");
    }

    if (_ressource_type == "unknown")
    {
        _return_code = 404;
        throw std::runtime_error("Not Found: " + _path);
    }

    if (std::remove(_path.c_str()) != 0)
    {
        _return_code = 500;
        throw std::runtime_error("Internal Server Error: cannot delete file");
    }
}