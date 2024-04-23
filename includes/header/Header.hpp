#ifndef HEADER_HPP
#define HEADER_HPP

#include <string>
#include <map>


class HeaderRequest {
    private:
        std::map<std::string, std::string> _headers;

    public:
        HeaderRequest();
        ~HeaderRequest();

        int                                 size() const; 
        std::map<std::string, std::string>  getHeaders() const;
        bool                                isSet(const std::string &key) const;
        void                                removeHeader(const std::string &key);
        int                                 getIndex(const std::string &key) const;
        std::string                         getHeader(const std::string &key) const;
        void                                addHeader(const std::string &key, const std::string &value);
        void                                updateHeader(const std::string &key, const std::string &value);
};

class HeaderResponse {
    private:
        std::map<std::string, std::string> _headers;

    public:
        HeaderResponse();
        ~HeaderResponse();

        int                                 size() const; 
        std::map<std::string, std::string>  getHeaders() const;
        bool                                isSet(const std::string &key) const;
        void                                removeHeader(const std::string &key);
        int                                 getIndex(const std::string &key) const;
        std::string                         getHeader(const std::string &key) const;
        void                                addHeader(const std::string &key, const std::string &value);
        void                                updateHeader(const std::string &key, const std::string &value);
};

#endif