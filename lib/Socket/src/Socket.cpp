#include "Socket.h"

Socket::Socket() {
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
}

Socket::~Socket() {
    close();
}

void Socket::host(int port, int backlog) {
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    bind(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    listen(socket_fd, backlog);

    std::cout << "Server is listening on port " << port << std::endl;

    accept();

}

void Socket::accept() {
    sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int client_fd = ::accept(socket_fd, (struct sockaddr*)&client_addr, &addr_len);

   char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, ip, sizeof(ip));
    std::cout << "Client connected " << ip << std::endl;
}

void Socket::close() {
    ::close(socket_fd);
    socket_fd = -1;
    std::cout << "Socket closed." << std::endl;
}

void Socket::connect(const std::string& host, int port) {
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, host.c_str(), &server_addr.sin_addr);

    ::connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    std::cout << "Connected to server at " << host << ":" << port << std::endl;
}

void Socket::disconnect() {
    close();
    std::cout << "Disconnected from server." << std::endl;
}