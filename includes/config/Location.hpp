// Location.hpp

#pragma once

#include <string>
#include <vector>
#include <map>

class Location {
    public:
        Location();
        ~Location();
    
    private:
        bool                        _autoindex;             // repertories listing
        std::string                 _path;                  // name of the route
        std::string                 _root;                  // path of the folder to route
        std::string                 _index;                 // default file to return
        std::string                 _redirect_url;          // redirection
        std::string                 _client_body_temp_path; // path of the client upload's folder
        std::string                 _client_max_body_size;  // body size limitaion
        std::vector<std::string>    _allowed_methods;       // allowed methods (GET, POST and/or DELETE)
        std::map<int, std::string>  _error_pages;           // map of error pages with their codes
};