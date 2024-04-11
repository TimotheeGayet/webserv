// utils.cpp

#include "../includes/utils.hpp"
#include "../includes/Globals.hpp"

void cleanup() {
    close(epoll_fd);
    for (std::vector<SocketInfo>::iterator it = sockets.begin(); it != sockets.end(); ++it) {
        close(it->socket_fd);
    }
}

void handle_signal(int signal) {
    (void)signal;
    if (is_running) {
        cleanup();
    }
    throw Server::ExitSignal();
}