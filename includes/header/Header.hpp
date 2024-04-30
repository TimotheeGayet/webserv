#ifndef HEADER_HPP
#define HEADER_HPP

#include <map>
#include <string>
#include <iostream>
#include <vector>
#include "../config/ServerConfig.hpp"
#include "../Globals.hpp"

struct AcceptElement {
    std::string							typeMIME;
    float								quality;
    std::map<std::string, std::string>	parameters;

    AcceptElement() : quality(1.0f) {}
};

class HeaderRequest {
	private:
		std::vector<AcceptElement>	_accept;
		std::string 				_connection;
		int							_content_length;
		std::string 				_transfer_encoding;

	public:
		HeaderRequest();
		~HeaderRequest();

		void 						handleAccept(const std::string& value);
		void 						handleConnection(const std::string& value);
		void 						handleContentLength(const std::string& value, int& return_code);
		void 						handleHost(const std::string& value, ServerConfig& server_config);
		void 						handleTransferEncoding(const std::string& value, int& return_code);

		std::vector<AcceptElement>	getAccept() const;
		std::string					getConnection() const;
		int 						getContentLength() const;
		std::string					getTransferEncoding() const;

};

#endif