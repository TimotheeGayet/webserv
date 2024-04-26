#ifndef HEADER_HPP
#define HEADER_HPP

#include <map>
#include <string>
#include <iostream>
#include <vector>
#include "../config/ServerConfig.hpp"
#include "../Globals.hpp"


class HeaderRequest {
	public:
		static void handleHost(const std::string& value, ServerConfig& server_config);
		static void handleContentLength(const std::string& value, int& return_code);
		static void handleTransferEncoding(const std::string& value, int& return_code);
		static void handleContentType(const std::string& value, int& return_code);
};

class HeaderResponse {
	public:


};

#endif