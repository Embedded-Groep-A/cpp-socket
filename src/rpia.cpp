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
    {{0x1E, 0x4C, 0xCF, 0x05}, "Thijs"},
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
        auto [type, data] = piBus.poll();
        if (type == MessageType::RGB) {
            int r = static_cast<int>(data[0]);
            int g = static_cast<int>(data[1]);
            int b = static_cast<int>(data[2]);
            std::cout << "RGB values: R=" << r << ", G=" << g << ", B=" << b << std::endl;
            std::string rgbString = std::to_string(r) + " " + std::to_string(g) + " " + std::to_string(b);
            socket.sendToServer(MessageType::RGB, rgbString);
        }
        if (type == MessageType::UID) {
            bool found = false;
            for (const auto& eigenaar : eigenaars) {
                if (std::equal(std::begin(eigenaar.uid), std::end(eigenaar.uid), data.begin())) {
                    std::cout << "UID matched: " << eigenaar.eigenaarNaam << std::endl;
                    piBus.send(MessageType::ACCEPT, eigenaar.eigenaarNaam);
                    piBus.send(MessageType::OPEN, "");
                    found = true;
                    break;
                }
            }
            if (!found) {
                std::cout << "UID not recognized" << std::endl;
                piBus.send(MessageType::REJECT, "");
            }
        } else if (type == MessageType::BEL) {
            std::cout << "tringelingeling" << std::endl;
        }

    }

        


    

    return 0;
}