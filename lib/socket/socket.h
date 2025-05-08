#ifndef SOCKET_H
#define SOCKET_H

#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


class Socket {
public:
    Socket();
    ~Socket();
    // Singleton
    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;
    Socket(Socket&&) = delete;
    Socket& operator=(Socket&&) = delete;
    // Server
    void host(int port, int backlog);
    void close();
    // Client
    void connect(const std::string& host, int port);
    void disconnect();






private:
    int socket_fd;
};


#endif // SOCKET_H