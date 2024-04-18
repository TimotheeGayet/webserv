// Globals.hpp

#pragma once

#include <vector>
#include <sys/socket.h>
#include "./server/Server.hpp"
#include "./config/GlobalConfig.hpp"

const size_t MAX_BODY_SIZE = 1024 * 1024; // 1 MB
const size_t MAX_URI_SIZE = 80000; // 80000 bytes (Safari max-url size based)

extern int                     epoll_fd;
extern std::vector<SocketInfo> sockets;
extern bool                    is_running;
extern std::string             illustration;
extern GlobalConfig            g_config;