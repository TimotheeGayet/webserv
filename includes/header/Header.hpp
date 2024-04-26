#ifndef HEADER_HPP
#define HEADER_HPP

#include <map>
#include <string>

class HeaderRequest {
    private:
        std::string			_host;
		std::string			_accept;
		std::string			_user_agent;
		std::string			_connection;
		std::string			_content_type;
		std::string			_content_length;
	
	public:
		HeaderRequest();
		~HeaderRequest();

		std::string			getHost() const;
		std::string			getAccept() const;
		std::string			getUserAgent() const;
		std::string			getConnection() const;
		std::string			getContentType() const;
		std::string			getContentLength() const;

		void				setContentType(const std::string& content_type);
		void				setContentLength(const std::string& content_length);
		void				setHost(const std::string& host);
		void				setAccept(const std::string& accept);
		void				setUserAgent(const std::string& user_agent);
		void				setConnection(const std::string& connection);
};

class HeaderResponse {
	private:
		std::string			_date;
		std::string			_server;
		std::string			_connection;
		std::string			_content_type;
		std::string			_content_length;

	public:
		HeaderResponse();
		~HeaderResponse();

		std::string			getDate() const;
		std::string			getServer() const;
		std::string			getConnection() const;
		std::string			getContentType() const;
		std::string			getContentLength() const;

		void				setDate(const std::string& date);
		void				setServer(const std::string& server);
		void				setConnection(const std::string& connection);
		void				setContentType(const std::string& content_type);
		void				setContentLength(const std::string& content_length);
};

#endif