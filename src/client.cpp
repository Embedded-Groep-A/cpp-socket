#include "Socket.h"
#include <iostream>

int main() {
    Socket socket;
    std::string host = "192.168.1.135";
    int port = 8181;

    socket.connect(host, port, "RPI");

    while (true) {
        std::string message = socket.pollServer();

        sleep(1);
        socket.sendToServer("test");
        
    }


    socket.disconnect();

    return 0;
}