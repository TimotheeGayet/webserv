// Globals.hpp

#pragma once

#include <vector>
#include <sys/socket.h>
#include "./server/Server.hpp"

extern int                     epoll_fd;
extern std::vector<SocketInfo> sockets;
extern bool                    is_running;
extern std::string             illustration;
extern std::string             php_cgi_path;