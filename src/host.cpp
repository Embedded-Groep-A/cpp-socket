#include "Socket.h"
#include <iostream>

int main() {
    Socket socket;
    int port = 8181;
    int backlog = 5;

    socket.host(port, backlog);

    while(true) {
        socket.accept();
        ClientMessage msg = socket.poll();
        if (msg.clientID.empty() || msg.message.empty()) {
            continue;
        }
        if (msg.type == MessageType::STATE) {
            if (msg.message == "ON") {
                std::cout << "Switching ON" << std::endl;
                socket.sendToClient(msg.clientID, MessageType::TEXT, "bing chilling");
            } else if (msg.message == "OFF") {
                std::cout << "Switching OFF" << std::endl;
                socket.sendToClient(msg.clientID, MessageType::TEXT, "kappa chungus");
            } else {
                std::cout << "Unknown command: " << message << std::endl;
            }
        }

    }
    return 0;
}