#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "../header/Header.hpp"
#include "../Globals.hpp"
#include "../request/Request.hpp"

class Response {
    private:
        Request& 				_request;   // Request object
        std::string 			_response;  // Response body

        std::string 			ErrorResponse(); 

	public:
        Response( Request& request );
        ~Response();

		std::string 			getResponse();
};

#endif