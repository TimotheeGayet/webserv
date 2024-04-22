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

		// Request parts attributes
		std::string 			_method; // GET, POST, DELETE
		std::string 			_uri; // method://host:port/path/to/file?query#fragment
		std::string 			_host; // www.example.com
		u_int16_t 				_port; // 80
		std::string 			_path; // /path/to/file
		std::string 			_file; // file.ext
		std::string 			_query;	// query (key=value&key2=value2)
		std::string 			_fragment; // fragment (#fragment)
		std::string 			_version; // HTTP/1.1
		std::string 			_body;
		std::string 			_response;

		// Request parts methods
		void 					isValidURI();
		void					bodyParsing();
		void					headerParsing();
		void 					locationParsing();
		std::string 			getResourceType();
		bool 					isLocation(const std::string& path);
		Location				getLocation(const std::string& path);

		// Headers
		Header 					_headers;
		void					setupHandlers();
		void					handleHost(const std::string& value);
		void					handleContentType(const std::string& value);
		void					handleContentLength(const std::string& value);
		void					handleTransferEncoding(const std::string& value);
		// Add more handlers here
		void					processHeaders(const std::map<std::string, std::string>& headers);

		// Utils
		long 					stringToLong(const std::string& str);

	public:
		Request(const std::string &msg);
		~Request();

		std::string 			getFile();
		std::string 			getPath();
		int 					getReturnCode() const;
		ServerConfig 			getServerConfig() const;

};

#endif