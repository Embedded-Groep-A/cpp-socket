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
        if (msg.type == MessageType::RGB) {
            socket.sendToClient("WEMOS", MessageType::RGB, msg.message);

        }

    }
    return 0;
}