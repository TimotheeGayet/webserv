#pragma once

#include "./Globals.hpp"
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
		std::string 						_query;
		std::string 						_fragment;
		std::string 						_version;
		std::string 						_body;
		std::map<std::string, std::string>	_headers;
		std::size_t 						_content_length;
		std::string							_transfer_encoding;

		void 		isValidURI();
		void    	findHost(const std::string& value);

	public:
		Request(const std::string &msg);
		~Request();

		void 		Answer();
		std::string getResponse();
		void		headerParsing();
		void		bodyParsing();
};
