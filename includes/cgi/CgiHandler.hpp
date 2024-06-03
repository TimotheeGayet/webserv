// CgiHandler.hpp

#pragma once

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string>
#include <sstream>
#include <cstring>
#include <sstream>
#include <stdint.h>
#include "../../includes/config/ServerConfig.hpp"
#include "../../includes/config/Location.hpp"
#include "../../includes/request/Request.hpp"


class CgiHandler {
    public:
        static std::string execute_cgi(const std::string& filename, const std::string& cgi_path, ServerConfig server, Location location, Request &request);
    private:
        static char **makeEnv(ServerConfig server, Location location, Request &request);
};