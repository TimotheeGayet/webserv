#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "../Globals.hpp"
#include "Header.hpp"
#include <string>
#include <map>

class Request
{
	private:
		std::string 			_req;
		int 					_return_code;
		ServerConfig 			_server_config;

		// Request parts
		std::string 			_method;
		std::string 			_uri;
		std::string 				_host;
		u_int16_t 					_port;
		std::string 				_path;
		std::string 				_file;
		std::string 				_query;
		std::string 				_fragment;
		std::string 			_version;
		std::string 			_body;
		std::string 			_response;
		Header 					_headers;

		void 					isValidURI();
		void					bodyParsing();
		void					headerParsing();
		void 					locationParsing();
		std::string 			getResourceType();
		void    				findHost(const std::string& value);
		bool 					isLocation(const std::string& path);
		Location				getLocation(const std::string& path);

	public:
		Request(const std::string &msg);
		~Request();

		std::string 			getFile();
		std::string 			getPath();
		std::string 			getPath() const;
		std::string 			getFile() const;
		int 					getReturnCode() const;
		ServerConfig 			getServerConfig() const;
};

#endif