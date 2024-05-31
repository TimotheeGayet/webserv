// CgiHandler.hpp

#pragma once

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string>
#include <sstream>

class CgiHandler {
    public:
        static std::string execute_cgi(const std::string& filename, const std::string& cgi_path);
};