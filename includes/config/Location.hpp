// Location.hpp

#pragma once

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <cstdlib>

class Location {
    public:
        Location();
        ~Location();

        void                        setPath(const std::string value);
        void                        setRoot(const std::string value);
        void                        setIndex(const std::string value);
        void                        setAutoindex(const std::string value);
        void                        setRedirectUrl(const std::string value);
        void                        setAllowedMethods(const std::string value);
        void                        setClientMaxBodySize(const std::string value);
        void                        setClientBodyTempPath(const std::string value);
        void                        setErrorPages(const std::map<int, std::string> value);
        void                        setLocationParam(const std::string key, const std::string value);
        
        bool                        getAutoindex() const;
        bool                        isConfigured() const;
        std::string                 getPath() const;
        std::string                 getRoot() const;
        std::string                 getIndex() const;
        std::string                 getRedirectUrl() const;
        std::string                 getClientMaxBodySize() const;
        std::string                 getClientBodyTempPath() const;
        std::map<int, std::string>  getErrorPages() const;
        std::vector<std::string>    getAllowedMethods() const;
    
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

