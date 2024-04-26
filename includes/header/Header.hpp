#ifndef HEADER_HPP
#define HEADER_HPP

#include <map>
#include <string>
#include <iostream>
#include <vector>
#include "../config/ServerConfig.hpp"
#include "../Globals.hpp"


class HeaderRequest {
	private:
		int							_content_length;
		std::string 				_connection;
		std::string 				_content_type;
		std::string 				_transfer_encoding;
		std::vector<std::string>	_accept;

	public:
		void handleHost(const std::string& value, ServerConfig& server_config);
		void handleContentLength(const std::string& value, int& return_code);
		void handleTransferEncoding(const std::string& value, int& return_code);
		void handleContentType(const std::string& value, int& return_code);
};

#endif