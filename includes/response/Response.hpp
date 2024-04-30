#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "../header/Header.hpp"
#include "../Globals.hpp"
#include "../request/Request.hpp"
#include <iostream>
#include <string>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

class Response {
    private:
        Request& 				_request;   // Request object
        std::string 			_response;  // Response body
        std::string 			_header;    // Response header

        std::string 			ErrorResponse(int err_code); 

	public:
        Response( Request& request );
        ~Response();

        Request& 				getRequest();
		std::string 			getResponse();
        std::string             generate_listing_html(const std::string &dir_path);
};

#endif