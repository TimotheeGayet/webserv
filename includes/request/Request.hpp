#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "../Globals.hpp"
#include "Header.hpp"
#include <string>
#include <map>

class Request
{
	private:
		ServerConfig 			_server_config;
		int 					_return_code;
		std::string 			_req;

		// Request parts
		std::string 			_method;
		std::string 			_uri;
		std::string 				_host;
		uint64_t 					_port;
		std::string 				_path;
		std::string 				_file;
		std::string 				_query;
		std::string 				_fragment;
		std::string 			_version;
		std::string 			_body;
		std::string 			_response;
		Header 					_headers;

		void 					isValidURI();
		void    				findHost(const std::string& value);
		std::string 			getResourceType();
		bool 					isLocation(const std::string& path);
		Location				getLocation(const std::string& path);
		void 					locationParsing();
		void					headerParsing();
		void					bodyParsing();

	public:
		Request(const std::string &msg);
		~Request();

		std::string 			getFile();
		std::string 			getResponse();
};

#endif