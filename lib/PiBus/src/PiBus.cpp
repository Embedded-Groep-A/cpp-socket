#include <PiBus.h>
#include <iostream>

PiBus::PiBus() {}

PiBus::~PiBus() {
    close(fd);
}

void PiBus::openSerial(const char* port, int baud) {
    fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);

    struct termios options;
    tcgetattr(fd, &options);
    cfsetispeed(&options, baud);
    cfsetospeed(&options, baud);
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB; // No parity
    options.c_cflag &= ~CSTOPB; // 1 stop bit
    options.c_cflag &= ~CSIZE;   // Clear current data size setting
    options.c_cflag |= CS8;      // 8 data bits
    tcsetattr(fd, TCSANOW, &options);
}

void PiBus::send(MessageType type, const char* data) {
    std::string message;
    if (type == MessageType::ACCEPT) {
        message = std::string("!") + data + "\r";

    } else if (type == MessageType::REJECT) {
        message = std::string("#") + data + "\r";
    } else {
        message = "[" + typeToString(type) + "] " + data;;
    }
    write(fd, message.c_str(), message.size());
    std::cout << "Sent to bus: " << typeToString(type) << message << std::endl;
}

std::pair<MessageType, std::string> PiBus::poll() {
    char buffer[1024];
    ssize_t bytes = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes > 0) {
        buffer[bytes] = '\0';
        std::string message(buffer);
        size_t pos = message.find(']');
        if (pos != std::string::npos) {
            std::string typeStr = message.substr(1, pos - 2);
            MessageType type = stringToType(typeStr);
            std::string data = message.substr(pos + 2);
            std::cout << "Received from bus: " << message << std::endl;
            return {type, data};
        }
    }
    return {MessageType::UNKNOWN, ""};
}
