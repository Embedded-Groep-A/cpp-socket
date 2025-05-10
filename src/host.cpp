#include "Socket.h"
#include <iostream>

int main() {
    Socket socket;
    int port = 8181;
    int backlog = 5;

    socket.host(port, backlog);

    while(true) {
        socket.accept();
        std::cout << "Polling..." << std::endl;
        auto [clientID, message] = socket.poll();

        if (clientID.empty() || message.empty()) {
            continue;
        }

        if (message == "test") {
            socket.sendToClient(clientID, "testACK");
        }

    }
    return 0;
}