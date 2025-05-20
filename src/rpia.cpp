#include "Socket.h"
#include "PiBus.h"
#include "Message.h"
#include <iostream>
#include <unistd.h> 

int main() {
    Socket socket;
    std::string host = "145.52.127.241";
    int port = 8181;
    socket.connect(host, port, "RPIA");

    PiBus piBus;
    piBus.openSerial("/dev/ttyS0", 115200);

    while (true) {
        piBus.send(MessageType::ACCEPT, "AHMED");
        sleep(6);
        piBus.send(MessageType::REJECT, "");
    }

        


    

    return 0;
}