// Globals.hpp

#pragma once

#include <vector>
#include <sys/socket.h>
#include "./server/Server.hpp"
#include "./config/GlobalConfig.hpp"

extern int                     epoll_fd;
extern std::vector<SocketInfo> sockets;
extern bool                    is_running;
extern std::string             illustration;
extern GlobalConfig&           g_config;