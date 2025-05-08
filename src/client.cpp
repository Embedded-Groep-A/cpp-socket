#include "../lib/Socket/Socket.h"
#include <iostream>

int main() {
    Socket socket;
    string host = "raspberrypi.local";
    int port = 8181;

    socket.connect(host, port);

    return 0;
}