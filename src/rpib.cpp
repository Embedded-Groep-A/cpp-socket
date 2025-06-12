#include "Socket.h"
#include <iostream>

typedef struct {
    std::string uid;
    const char* eigenaarNaam;
} Eigenaar;

Eigenaar eigenaars[] = {
    {"77 da ac 02", "Cas"},
    {"1e 4c cf 05", "Thijs"},
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
            std::string uid(msg.message);
            std::cout << "Received UID: " << uid << std::endl;
            bool found = false;
            for (const auto& eigenaar : eigenaars) {
                if (eigenaar.uid == uid) {
                    std::cout << "UID matched: " << eigenaar.eigenaarNaam << std::endl;
                    const char* eigenaarNaamWithCR = (std::string(eigenaar.eigenaarNaam) + "\r").c_str();
                    socket.sendToClient(msg.clientID, MessageType::ACCEPT, eigenaarNaamWithCR);
                    socket.sendToClient(msg.clientID, MessageType::OPEN, "");
                    found = true;
                    break;
                }
            }
            if (!found) {
                std::cout << "UID not recognized" << std::endl;
                socket.sendToClient(msg.clientID, MessageType::REJECT, "");
            }
        }
    }
    return 0;
}   
