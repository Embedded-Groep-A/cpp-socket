#include "Socket.h"
#include <iostream>

int main() {
    Socket socket;
    int port = 8181;
    int backlog = 5;

    socket.host(port, backlog);

    while(true) {
        std::cout << "1" << std::endl;
        socket.accept();
        auto [clientID, message] = socket.poll();
        std::cout << "5" << std::endl;
        if (clientID.empty() || message.empty()) {
            continue;
        }
        std::cout << "6" << std::endl;
        if (message == "test") {
            socket.sendToClient(clientID, "testACK");
        }

    }
    return 0;
}