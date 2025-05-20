#include "Socket.h"
#include "PiBus.h"
#include "Message.h"
#include <iostream>
#include <unistd.h>

typedef struct {
    uint8_t uid[4];
    const char* eigenaarNaam;
} Eigenaar;

Eigenaar eigenaars[] = {
    {{0x77, 0xDA, 0xAC, 0x02}, "Cas"},
    {{0xB1, 0xFF, 0x74, 0x1D}, "Thijs"},
   // {{0x64, 0x81, 0xE6, 0x03}, "Ahmed"}
};

int main() {
    Socket socket;
    std::string host = "145.52.127.241";
    int port = 8181;
    socket.connect(host, port, "RPIA");

    PiBus piBus;
    piBus.openSerial("/dev/ttyS0", 9600);

    while (true) {

        std::string data = piBus.rawRead();
        if (!data.empty()) {
            std::cout << "Received from PiBus: " << data << std::endl;
        }
    }

        


    

    return 0;
}