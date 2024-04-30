#include "../../includes/request/Request.hpp"

void Request::uploadFile()
{
    if (this->_ressource_type == "directory")
    {
        this->_return_code = 400;
        throw std::runtime_error("Bad Request: cannot upload file to a directory");
    }
    
    std::string path_to_file = this->_path.substr(0, this->_path.find_last_of('/'));

    // Check if the path to the file exists
    if (getResourceType(path_to_file.c_str()) != "directory" || getResourceType(this->_path.c_str()) == "root")
    {
        this->_return_code = 404;
        throw std::runtime_error("Invalid path for file uploading: " + this->_path);
    }

    // Create the file first if it doesn't exists
    if (getResourceType(this->_path.c_str()) == "unknown")
    {
        std::ofstream new_file(this->_path.c_str(), std::ofstream::out);
        new_file.close();
    }

    std::ofstream file;
    file.open(this->_path.c_str(), std::ifstream::app);

    if (!file.is_open())
    {
        this->_return_code = 500;
        throw std::runtime_error("Internal Server Error: cannot open file for writing");
    }

    file << this->_body;
    file.close();

    this->_file = this->_path.substr(this->_path.find_last_of('/') + 1);
}