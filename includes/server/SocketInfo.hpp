// SocketInfo.hpp

#pragma once

#include <cstdlib>

struct SocketInfo {
    int socket_fd;
    u_int16_t port;
};