#include "Socket.h"
#include <iostream>

typedef struct {
    std::string uid;
    const char* eigenaarNaam;
} Eigenaar;

Eigenaar eigenaars[] = {
    {"77 DA AC 02", "Cas"},
    {"1E 4C CF 05", "Thijs"},
   // {{0x64, 0x81, 0xE6, 0x03}, "Ahmed"}
};

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
        // FORWARD RGB VALUE FROM RPIA TO WEMOS
        if (msg.type == MessageType::RGB) {
            socket.sendToClient("WEMOSRGB", MessageType::RGB, msg.message);
        // SEND ON TO RGB WHEN RECEIVNG MOVEMENT  
        } else if (msg.type == MessageType::STATE) {
            if (msg.message == "BEWEGING") {
                socket.sendToClient("WEMOSRGB", MessageType::STATE, "ON");
            } else if (msg.message == "STOEL") {
                std::cout << "Received STOEL command" << std::endl;
                socket.sendToClient("WEMOSR", MessageType::STATE, "STOEL");
            }
        } else if (msg.type == MessageType::UID) {
            std::string uid = msg.message;
            std::cout << "Received UID: " << uid << std::endl;

            // Check if the UID matches any owner
            for (const auto& eigenaar : eigenaars) {
                if (uid == eigenaar.uid) {
                    std::cout << "UID matched: " << eigenaar.eigenaarNaam << std::endl;
                    socket.sendToClient("RPIA", MessageType::ACCEPT, eigenaar.eigenaarNaam);
                    socket.sendToClient("RPIA", MessageType::OPEN, "");
                    break;
                }
            }
        } else {
            std::cout << "Unknown message type: " << static_cast<int>(msg.type) << std::endl;
        }
    }

    return 0;
}   
