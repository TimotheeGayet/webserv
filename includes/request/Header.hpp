#ifndef HEADER_HPP
#define HEADER_HPP

#include <string>
#include <map>


class Header {
    private:
        std::map<std::string, std::string> _headers;

    public:
        Header();
        ~Header();

        void updateHeader(const std::string &key, const std::string &value);
        void removeHeader(const std::string &key);
        std::string getHeader(const std::string &key) const;
        int getIndex(const std::string &key) const;
        std::map<std::string, std::string> getHeaders() const;

        bool isSet(const std::string &key) const;
        int size() const;

        std::map<std::string, std::string>::iterator begin();
        std::map<std::string, std::string>::iterator end();
};

#endif