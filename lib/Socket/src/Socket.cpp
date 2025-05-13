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
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(socket_fd, &read_fds);

    timeval timeout{};
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    if (select(socket_fd + 1, &read_fds, nullptr, nullptr, &timeout) > 0 &&
        FD_ISSET(socket_fd, &read_fds)) {
        
        sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        int client_fd = ::accept(socket_fd, (struct sockaddr*)&client_addr, &addr_len);

        if (client_fd >= 0) {
            char ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &client_addr.sin_addr, ip, sizeof(ip));
            std::cout << "Client connecting from " << ip << std::endl;

            char ID[9] = {0};
            int bytes_received = recv(client_fd, ID, 8, 0);
            if (bytes_received > 0) {
                ID[bytes_received] = '\0';
                std::string idStr(ID);
                std::cout << "Received ID: " << idStr << std::endl;

                clientIDs[client_fd] = idStr;
                send(client_fd, "ACK", 3, 0);
                clients.push_back(client_fd);

                std::cout << "Client fd " << client_fd << " added to poll list." << std::endl;
            } else {
                ::close(client_fd);
            }
        }
    }
}


std::pair<std::string, std::string> Socket::poll() {
    fd_set read_fds;
    FD_ZERO(&read_fds);
    int max_fd = -1;

    for (int fd : clients) {
        if (fd >= 0) {
            FD_SET(fd, &read_fds);
            if (fd > max_fd) max_fd = fd;
        }
    }

    if (max_fd < 0) {
        return {};
    }

    timeval timeout{};
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    int ready = select(max_fd + 1, &read_fds, nullptr, nullptr, &timeout);
    if (ready <= 0) {
        return {};
    }

    std::pair<std::string, std::string> result;

    // Iterate through the list of connected clients
    for (auto it = clients.begin(); it != clients.end(); ) {
        int fd = *it; // Get the file descriptor for the current client

        // Check if the current client's file descriptor is ready for reading
        if (FD_ISSET(fd, &read_fds)) {
            char buffer[1024]; // Buffer to store the incoming message
            ssize_t bytes = recv(fd, buffer, sizeof(buffer), 0); // Receive data from the client

            // If the received bytes are less than or equal to 0, the client has disconnected or an error occurred
            if (bytes <= 0) {
                std::string clientID = clientIDs[fd]; // Retrieve the client ID before disconnecting
                it = clients.erase(it); // Remove the client from the list of connected clients
                disconnectClient(fd); // Disconnect the client and clean up resources
                std::cout << "Client " << clientID << " disconnected due to error or closure." << std::endl;
                continue; // Skip the rest of the loop and move to the next client
            } else {
                // If data is received successfully, process the message
                std::string message(buffer, bytes); // Convert the received data into a string
                std::string clientID = clientIDs[fd]; // Retrieve the client ID associated with the file descriptor
                std::cout << "Received message from client " << clientID << ": " << message << std::endl;

                // Store the client ID and message in the result pair
                result = {clientID, message};
            }
        }
        ++it; // Move to the next client in the list
    }

    return result;
}



void Socket::close() {
    ::close(socket_fd);
    socket_fd = -1;
    std::cout << "Socket closed." << std::endl;
}

void Socket::sendToClient(const std::string& clientID, const std::string& message) {
    for (const auto& pair : clientIDs) {
        if (pair.second == clientID) {
            int client_fd = pair.first;
            send(client_fd, message.c_str(), message.size(), 0);
            std::cout << "Sent message to client " << clientID << ": " << message << std::endl;
            return;
        }
    }

    std::cout << "Client ID not found: " << clientID << std::endl;
}

void Socket::connect(const std::string& host, int port, const std::string& id) {
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, host.c_str(), &server_addr.sin_addr);

    ::connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    std::cout << "Connecting to server at " << host << ":" << port << std::endl;
    // Send ID to server
    send(socket_fd, id.c_str(), id.size(), 0);
    // Wait for ACK
    char buffer[1024];
    int bytes_received = recv(socket_fd, buffer, sizeof(buffer), 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        if (strcmp(buffer, "ACK") == 0) {
            std::cout << "Connected to server." << std::endl;
        }
    }
}

void Socket::disconnect() {
    close();
    std::cout << "Disconnected from server." << std::endl;
}

void Socket::sendToServer(const std::string& message) {
    send(socket_fd, message.c_str(), message.size(), 0);
    std::cout << "Sent message to server: " << message << std::endl;
}

std::string Socket::pollServer() {
    char buffer[1024];
    ssize_t bytes = recv(socket_fd, buffer, sizeof(buffer), 0);
    if (bytes > 0) {
        std::cout << "Received message from server: " << buffer << std::endl;
        return std::string(buffer, bytes);
    }
    return {};
}

void Socket::disconnectClient(int client_fd) {
    auto it = std::find(clients.begin(), clients.end(), client_fd);
    if (it != clients.end()) {
        std::string clientID = clientIDs[client_fd];
        clients.erase(it);
        clientIDs.erase(client_fd);
        ::close(client_fd);
        std::cout << "Client " << clientID << " disconnected." << std::endl;
    } else {
        std::cout << "disconnectClient: client not found" << std::endl;
    }
}