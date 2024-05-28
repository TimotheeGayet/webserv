// Globals.hpp

#pragma once

#include <vector>
#include <sys/socket.h>
#include "server/SocketInfo.hpp"
#include "./config/GlobalConfig.hpp"

extern size_t                  MAX_BODY_SIZE;
extern size_t                  MAX_URI_SIZE;
extern std::string             php_cgi_path;

extern int                     epoll_fd;
extern std::vector<SocketInfo> sockets;
extern bool                    is_running;
extern std::string             illustration;
extern GlobalConfig            g_config;