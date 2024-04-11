// Server.cpp

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <signal.h>
#include <cstdlib>

struct SocketInfo;

std::vector<SocketInfo> sockets;
int epoll_fd;

struct SocketInfo {
    int socket_fd;
    uint16_t port;
};

SocketInfo initializeSocket(uint16_t port) {
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "error: socket failed: ";
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt))) {
        std::cerr << "error: setsockopt";
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);


    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0) {
        std::cerr << "error: bind failed";
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        std::cerr << "error: listen";
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << port << std::endl;

    SocketInfo socket_info = {server_fd, port};
    return socket_info;
}

void handleHttpRequest(int client_fd, const std::string& request) {
    std::istringstream iss(request);
    std::string firstLine;
    std::getline(iss, firstLine);
    std::istringstream issFirstLine(firstLine);
    std::string method, path, http_version;
    issFirstLine >> method >> path >> http_version;

    std::cout << "Requête pour : " << path << std::endl;
    std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nTIMOTE A TOI DE JOUER! TIMOTE ATTAQUE ECLAIR! CELA ECHOUE...\r\n";

    send(client_fd, response.c_str(), response.size(), 0);
}

void cleanup() {
    close(epoll_fd);

    for (std::vector<SocketInfo>::iterator it = sockets.begin(); it != sockets.end(); ++it) {
        close(it->socket_fd);
    }
}

void handle_signal(int signal) {
    (void)signal;
    cleanup();
    exit(EXIT_SUCCESS);
}

int main() {
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    epoll_fd = epoll_create(1);
    if (epoll_fd == -1) {
        std::cerr << "error: epoll_create" << std::endl;
        cleanup();
        exit(EXIT_FAILURE);
    }

    std::vector<uint16_t> ports;
    ports.push_back(8080);
    ports.push_back(8081);

    for (std::vector<uint16_t>::iterator it = ports.begin(); it != ports.end(); ++it) {
        SocketInfo socket_info = initializeSocket(*it);
        sockets.push_back(socket_info);

        struct epoll_event event;
        event.events = EPOLLIN;
        event.data.fd = socket_info.socket_fd;
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_info.socket_fd, &event) == -1) {
            std::cerr << "error: epoll_ctl EPOLL_CTL_ADD" << std::endl;
            cleanup();
            exit(EXIT_FAILURE);
        }
    }

    while (true) {
        struct epoll_event events[10];

        int nfds = epoll_wait(epoll_fd, events, 10, -1);
        if (nfds == -1) {
            std::cerr << "error: epoll_wait" << std::endl;
            cleanup();
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < nfds; ++i) {
            int fd = events[i].data.fd;

            uint16_t port = 0;
            for (std::vector<SocketInfo>::iterator it = sockets.begin(); it != sockets.end(); ++it) {
                if (it->socket_fd == fd) {
                    port = it->port;
                    break;
                }
            }

            if (port != 0) {
                struct sockaddr_in client_address;
                socklen_t client_address_len = sizeof(client_address);
                int client_fd = accept(fd, (struct sockaddr *)&client_address, &client_address_len);
                if (client_fd == -1) {
                    std::cerr << "error: accept" << std::endl;
                    cleanup();
                    exit(EXIT_FAILURE);
                }

                struct epoll_event event;
                event.events = EPOLLIN;
                event.data.fd = client_fd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event) == -1) {
                    std::cerr << "error: epoll_ctl EPOLL_CTL_ADD" << std::endl;
                    cleanup();
                    exit(EXIT_FAILURE);
                }
            } else {
                char buffer[1024];
                int bytes_received = recv(fd, buffer, sizeof(buffer), 0);
                if (bytes_received <= 0) {
                    if (bytes_received == 0) {
                        std::cout << "Client disconnected" << std::endl;
                    } else {
                        std::cerr << "error: recv" << std::endl;
                    }
                    close(fd);
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
                } else {
                    std::string request(buffer, bytes_received);
                    handleHttpRequest(fd, request);

                    close(fd);
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
                }
            }
        }
    }

    close(epoll_fd);

    for (std::vector<SocketInfo>::iterator it = sockets.begin(); it != sockets.end(); ++it) {
        close(it->socket_fd);
    }

    return 0;
}