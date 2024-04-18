#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "../Globals.hpp"
#include "Header.hpp"
#include <string>
#include <map>

class Request
{
	private:
		int 								_return_code;
		ServerConfig 						_server_config;
		std::string 						_req;
		std::string 						_method;
		std::string 						_uri;
		std::string 						_host;
		std::size_t 						_port;
		std::string 						_path;
		std::string 						_file;
		std::string 						_query;
		std::string 						_fragment;
		std::string 						_version;
		std::string 						_body;
		std::string 						_response;
		Header 								_headers;

		void 		isValidURI();
		void    	findHost(const std::string& value);
		std::string getResourceType();

	public:
		Request(const std::string &msg);
		~Request();

		std::string getResponse();
		void		headerParsing();
		void		bodyParsing();
};

#endif