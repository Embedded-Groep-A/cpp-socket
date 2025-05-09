#include "Socket.h"
#include <iostream>

int main() {
    Socket socket;
    std::string host = "192.168.1.135";
    int port = 8181;

    socket.connect(host, port, "RPIB");

    while (true) {
        std::string message = socket.pollServer();

        sleep(1);
        socket.sendToServer("test");
        std::cout << "Sent message to server: test" << std::endl;
        
    }

    std::cout << "pp" << std::endl;
    socket.disconnect();

    return 0;
}