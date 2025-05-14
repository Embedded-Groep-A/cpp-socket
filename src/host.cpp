#include "Socket.h"
#include <iostream>

int main() {
    Socket socket;
    int port = 8181;
    int backlog = 5;

    socket.host(port, backlog);

    while(true) {
        socket.accept();
        auto [clientID, type, message] = socket.poll();
        if (clientID.empty() || message.empty()) {
            continue;
        }
        if (type == MessageType::STATE) {
            if (message == "ON") {
                std::cout << "Switching ON" << std::endl;
                socket.sendToClient(clientID, MessageType::TEXT, "bing chilling");
            } else if (message == "OFF") {
                std::cout << "Switching OFF" << std::endl;
                socket.sendToClient(clientID, MessageType::TEXT, "kappa chungus");
            } else {
                std::cout << "Unknown command: " << message << std::endl;
            }
        }

    }
    return 0;
}