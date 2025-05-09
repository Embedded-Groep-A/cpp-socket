#include "Socket.h"
#include <iostream>

int main() {
    Socket socket;
    std::string host = "192.168.1.135";
    int port = 8181;

    socket.connect(host, port, "RPI");

    while (true) {
        // KEEP CHECKING FOR MESSAGES
        std::string message = socket.pollServer();
        if (message.empty()) {
            continue;
        }
        //////////////////////////////
        socket.sendToServer("test");
        
    }


    socket.disconnect();

    return 0;
}