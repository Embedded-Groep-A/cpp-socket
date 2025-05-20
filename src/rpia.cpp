#include "Socket.h"
#include "PiBus.h"
#include "Message.h"
#include <iostream>

int main() {
    Socket socket;
    std::string host = "145.52.127.241";
    int port = 8181;
    socket.connect(host, port, "RPIA");

    PiBus piBus;
    piBus.openSerial("/dev/ttyS0", 115200);

    while (true) {
        piBus.send(MessageType::TEXT, "TEST");
    }

        


    

    return 0;
}