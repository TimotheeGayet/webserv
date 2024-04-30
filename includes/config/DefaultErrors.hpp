// DefaultErrors.hpp

#pragma once

#include <string>

class DefaultErrors {
    public:
        DefaultErrors();
        ~DefaultErrors();

        std::string getError(int code);
        std::string getErrorPage(int code);

        const std::string &get400() const { return _400; }
        const std::string &get401() const { return _401; }
        const std::string &get403() const { return _403; }
        const std::string &get404() const { return _404; }
        const std::string &get405() const { return _405; }
        const std::string &get406() const { return _406; }
        const std::string &get408() const { return _408; }
        const std::string &get410() const { return _410; }
        const std::string &get411() const { return _411; }
        const std::string &get413() const { return _413; }
        const std::string &get414() const { return _414; }
        const std::string &get415() const { return _415; }
        const std::string &get416() const { return _416; }
        const std::string &get417() const { return _417; }
        const std::string &get418() const { return _418; }
        const std::string &get429() const { return _429; }
        const std::string &get500() const { return _500; }
        const std::string &get501() const { return _501; }
        const std::string &get502() const { return _502; }
        const std::string &get503() const { return _503; }
        const std::string &get504() const { return _504; }
        const std::string &get505() const { return _505; }

    private:
        const std::string _400;
        const std::string _401;
        const std::string _403;
        const std::string _404;
        const std::string _405;
        const std::string _406;
        const std::string _408;
        const std::string _410;
        const std::string _411;
        const std::string _413;
        const std::string _414;
        const std::string _415;
        const std::string _416;
        const std::string _417;
        const std::string _418;
        const std::string _429;
        const std::string _500;
        const std::string _501;
        const std::string _502;
        const std::string _503;
        const std::string _504;
        const std::string _505;
};