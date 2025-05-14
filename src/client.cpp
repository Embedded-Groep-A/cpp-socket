#include "Socket.h"
#include <iostream>

int main() {
    Socket socket;
    std::string host = "192.168.1.203";
    int port = 8181;

    socket.connect(host, port, "RPIB");

    while (true) {
        sleep(1);
        socket.sendToServer(MessageType::STATE, "ON");

        auto [type, message] = socket.pollServer();
    }

    socket.disconnect();

    return 0;
}