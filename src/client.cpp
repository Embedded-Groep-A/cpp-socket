#include "Socket.h"
#include <iostream>

int main() {
    Socket socket;
    std::string host = "192.168.1.135";
    int port = 8181;

    socket.connect(host, port);

    while (true) {
        // Keep the client running to maintain the connection
    }

    return 0;
}