#include "Socket.h"
#include "PiBus.h"
#include "Message.h"
#include <iostream>
#include <unistd.h>

int main() {
    Socket socket;
    std::string host = "145.52.127.103";
    int port = 8181;
    socket.connect(host, port, "RPIA");

    PiBus piBus;
    piBus.openSerial("/dev/ttyS0", 9600);

    while (true) {
        //BUS NAAR SERVER
        auto [type, data] = piBus.poll();
        if (type == MessageType::RGB) {
            int r = static_cast<int>(data[0]);
            int g = static_cast<int>(data[1]);
            int b = static_cast<int>(data[2]);
            std::cout << "RGB values: R=" << r << ", G=" << g << ", B=" << b << std::endl;
            std::string rgbString = std::to_string(r) + " " + std::to_string(g) + " " + std::to_string(b);
            socket.sendToServer(MessageType::RGB, rgbString);
        } else if (type != MessageType::UNKNOWN) {
            socket.sendToServer(type, data.c_str());
        } 
        auto [serverType, serverMessage] = socket.pollServer();
        if (serverType != MessageType::UNKNOWN) {
            piBus.send(serverType, serverMessage.c_str());
        }

    }
    return 0;
}