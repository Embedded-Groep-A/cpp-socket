#include "../../lib/Socket/Socket.h"
#include <iostream>

int main() {
    Socket socket;
    int port = 8181;
    int backlog = 5;

    // Host a server
    socket.host(port, backlog);
    std::cout << "Server is hosting on port " << port << " with a backlog of " << backlog << std::endl;

    // Close the server socket
    socket.close();
    std::cout << "Server socket closed." << std::endl;

    return 0;
}