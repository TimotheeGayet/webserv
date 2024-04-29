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
		int							_content_length;
		std::string 				_connection;
		std::string 				_content_type;
		std::string 				_transfer_encoding;
		std::vector<AcceptElement>	_accept;

	public:
		HeaderRequest();
		~HeaderRequest();

		void 						handleHost(const std::string& value, ServerConfig& server_config);
		void 						handleAccept(const std::string& value);
		void 						handleConnection(const std::string& value);
		void 						handleContentLength(const std::string& value, int& return_code);
		void 						handleTransferEncoding(const std::string& value, int& return_code);
		void 						handleContentType(const std::string& value, int& return_code);

		int 						getContentLength() const;
		std::string					getConnection() const;
		std::string					getContentType() const;
		std::string					getTransferEncoding() const;
		std::vector<AcceptElement>	getAccept() const;

};

#endif