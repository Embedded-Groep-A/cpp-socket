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
    std::pair<int, std::string> poll();
    void accept();
    void close();

    void sendToClient(const std::string& clientID, const std::string& message);
    // Client
    void connect(const std::string& host, int port, const std::string& id);
    void disconnect();

    void sendToServer(const std::string& message);
    std::string pollServer();






private:
    int socket_fd;
    std::vector<int> clients;
    std::unordered_map<int, std::string> clientIDs; 

    void disconnectClient(int client_fd);
};


#endif // SOCKET_H