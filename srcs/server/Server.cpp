// Server.cpp

#include "../../includes/server/Server.hpp"
#include "../../includes/Globals.hpp"
#include "../../includes/utils.hpp"
#include "../../includes/request/Request.hpp"
#include "../../includes/response/Response.hpp"

// ************************************************************************************************ //

Server::Server(GlobalConfig conf, std::vector<u_int16_t> ports)
    : _GlobalConfig(conf),
      _ports(ports)
      
{
    for (std::vector<u_int16_t>::iterator it = _ports.begin(); it != _ports.end(); ++it) {
        if (*it < 1024) {
            std::cerr << "error: ports below 1024 are reserved" << std::endl;
            throw Server::ExitError();
        }
    }
}

Server::~Server() {
    cleanup();
}

SocketInfo Server::initializeSocket(u_int16_t port) {
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;

    // AF_INET -> IPv4 network
    // SOCK_STREAM -> socket type TCP
    // 0 -> set protocol to use
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "error: socket failed: ";
        throw Server::ExitError();
    }

    // SOL_SOCKET -> apply option to the socket itself, not on another protocol
    // SO_REUSEADDR -> multiples sockets can listen on the same ip adress / port at the same time -> fast reboot
    // SO_REUSEPORT -> multiples sockets can listen on the same port, even if the ip is different
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        std::cerr << "error: setsockopt";
        throw Server::ExitError();
    }

    address.sin_family = AF_INET; // adress type -> IPv4
    address.sin_addr.s_addr = INADDR_ANY; // listen on every network interfaces
    address.sin_port = htons(port); // listend port

    // Link socket to adress and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        std::cerr << "error: bind failed";
        throw Server::ExitError();
    }

    if (listen(server_fd, 3) < 0) {
        std::cerr << "error: listen";
        throw Server::ExitError();
    }

    std::cout << "Server listening on port " << port << std::endl;

    SocketInfo socket_info = {server_fd, port};
    return socket_info;
}

int Server::run() {
    epoll_fd = epoll_create(1);
    if (epoll_fd == -1) {
        std::cerr << "error: epoll_create" << std::endl;
        throw Server::ExitError();
    }

    for (std::vector<u_int16_t>::iterator it = _ports.begin(); it != _ports.end(); ++it) {
        SocketInfo socket_info = initializeSocket(*it);
        sockets.push_back(socket_info);

        struct epoll_event event;
        event.events = EPOLLIN; // listen input events
        event.data.fd = socket_info.socket_fd;
        // add socket to epoll instance
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_info.socket_fd, &event) == -1) {
            std::cerr << "error: epoll_ctl EPOLL_CTL_ADD" << std::endl;
            cleanup();
            throw Server::ExitError();
        }
    }

    while (true) {
        try {
            struct epoll_event events[10]; // 10 events max

            int nfds = epoll_wait(epoll_fd, events, 10, -1);
            if (nfds == -1) {
                std::cerr << "error: epoll_wait" << std::endl;
                cleanup();
                throw Server::ExitError();
            }

            for (int i = 0; i < nfds; ++i) {
                int fd = events[i].data.fd;

                // Find the port associated with the file descriptor
                u_int16_t port = 0;
                for (std::vector<SocketInfo>::iterator it = sockets.begin(); it != sockets.end(); ++it) {
                    if (it->socket_fd == fd) {
                        port = it->port;
                        break;
                    }
                }

                if (port != 0) {
                    // file descriptor associated with a port -> new client connection
                    struct sockaddr_in client_address;
                    socklen_t client_address_len = sizeof(client_address);
                    int client_fd = accept(fd, (struct sockaddr *)&client_address, &client_address_len);
                    if (client_fd == -1) {
                        std::cerr << "error: accept" << std::endl;
                        cleanup();
                        throw Server::ExitError();
                    }
                    // Add the client socket to the epoll instance
                    struct epoll_event event;
                    event.events = EPOLLIN; // listen input events
                    event.data.fd = client_fd;
                    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event) == -1) {
                        std::cerr << "error: epoll_ctl EPOLL_CTL_ADD" << std::endl;
                        cleanup();
                        throw Server::ExitError();
                    }
                } else {
                    // file descriptor associated with a client socket -> data received
                    char buffer[1024];
                    int bytes_received = recv(fd, buffer, sizeof(buffer), 0);
                    if (bytes_received <= 0) {
                        // no data received -> client disconnected or error
                        
                        if (bytes_received == 0) {
                            std::cout << "Client disconnected" << std::endl << std::endl;
                        } else {
                            std::cerr << "error: recv" << std::endl;
                        }
                        close(fd);
                        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
                    } else {
                        // data received -> handle the HTTP request
                        std::string request(buffer, bytes_received);

                        Request req(request);
                        Response res(req);
                        std::string response = res.getResponse();

                        // Check the connection type
                        if (req.getHeader().getConnection() == "close" || req.getReturnCode() != 200 || \
                            res.getStatusCode() != 200 || req.getDoRedirect() == true) {
                            int bytes_sent = send(fd, response.c_str(), response.size(), 0);
                            if (bytes_sent != static_cast<int>(response.size())){
                                std::cerr << "error: send" << std::endl;
                            }
                            close(fd);
                            epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
                            std::cout << "Connection closed" << std::endl << std::endl;
                        }
                        else if (req.getHeader().getConnection() == "keep-alive") {
                            int bytes_sent = send(fd, response.c_str(), response.size(), 0);
                            if (bytes_sent != static_cast<int>(response.size())){
                                std::cerr << "error: send" << std::endl;
                            }

                        }
                         else {
                            std::cerr << "Invalid Connection: " << req.getHeader().getConnection() << std::endl << std::endl;
                            close(fd);
                            epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);

                        }
                    }
                }
            }
        }
        catch (const Server::ExitSignal& e) {
            std::cerr << e.what() << std::endl;
            cleanup();
            return 0;
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            cleanup();
            return 1;
        }
    }
    return 0;
}