// globals.cpp

#include "../includes/Globals.hpp"

int epoll_fd;
std::vector<SocketInfo> sockets;
bool is_running = false;