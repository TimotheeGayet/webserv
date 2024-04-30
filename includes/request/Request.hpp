#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "../Globals.hpp"
#include "../header/Header.hpp"
#include <string>
#include <map>



class Request
{
	private:
		std::string 			_req;
		int 					_return_code;
		ServerConfig 			_server_config;
		Location				_location;

		// Request parts attributes
		std::string 			_method; 			// GET, POST, DELETE
		std::string 			_uri; 				// method://host:port/path/to/file?query#fragment
		std::string 			_host; 				// www.example.com
		u_int16_t 				_port; 				// 80
		std::string 			_path; 				// /path/to/file
		std::string 			_file; 				// file.ext
		std::string 			_ressource_type;	// "directory", "file", "root", ""
		std::string 			_query;				// query (key=value&key2=value2)
		std::string 			_fragment; 			// fragment (#fragment)
		std::string 			_version; 			// HTTP/1.1
		HeaderRequest 			_header;
		std::string 			_body;
		std::string 			_response;

		// Request parsing methods
		void 					isValidURI();
		void 					uploadFile();
		void 					resolvePath();
		void					bodyParsing();
		HeaderRequest			headerParsing();
		void 					locationParsing();
		void 					resolveDirectoryPath();
		std::string 			getResourceType(const char *path);
		bool 					isLocation(const std::string& path);
		long 					stringToLong(const std::string& str);
		Location				findLocation(const std::string& path);

	public:
		Request(const std::string &msg);
		~Request();

		std::string 			getFile();
		std::string 			getPath();
		int 					getReturnCode() const;
		ServerConfig 			getServerConfig() const;
		Location				getLocation() const;
		HeaderRequest 			getHeader() const;
		std::string 			getMethod() const;

};

#endif