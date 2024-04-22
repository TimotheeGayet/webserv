#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Header.hpp"
#include "../Globals.hpp"
#include "Request.hpp"

class Response {
    private:
        Request& 				_request; // Request object
        Header 					_headers; // Response headers
        std::string 			_response; // Response body

	public:
        Response( Request& request );
        ~Response();

		std::string 			getResponse();
};

#endif