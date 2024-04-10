#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>

class Request
{
private:
	std::string _req;
	std::string _method;
	std::string _uri;
	std::string _version;
	std::string _body;

public:
	// Constructors
	Request(std::string &msg); // We will need to replace msg by the config file object as a parameter
	~Request();

	void Answer();
	bool isValidURI(const std::string &uri);
};

#endif
