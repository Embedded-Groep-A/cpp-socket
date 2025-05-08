#include "Socket.h"
#include <iostream>

int main() {
    Socket socket;
    std::string host = "raspberrypi.local";
    int port = 8181;

    socket.connect(host, port);

    return 0;
}