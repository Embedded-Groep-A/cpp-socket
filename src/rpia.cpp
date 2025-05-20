#include "Socket.h"
#include "PiBus.h"
#include <iostream>

int main() {
    Socket socket;
    std::string host = "192.168.1.203";
    int port = 8181;
    socket.connect(host, port, "RPIA");

    PiBus piBus;
    piBus.openSerial("/dev/ttyS0", 115200);

    while (true) {
        auto [type, data] = piBus.poll();
        if (type == MessageType::UNKNOWN) {
            continue;
        }
        


    

    return 0;
}