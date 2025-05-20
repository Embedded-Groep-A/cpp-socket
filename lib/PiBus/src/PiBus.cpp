#include <PiBus.h>
#include <iostream>
#include <sys/socket.h>

PiBus::PiBus() {}

PiBus::~PiBus() {
    close(fd);
}

void PiBus::openSerial(const char* port, int baud) {
    fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd < 0) {
        perror("open");
        return;
    }

    struct termios options;
    memset(&options, 0, sizeof(options));

    tcgetattr(fd, &options);

    // Set baud rate
    cfsetispeed(&options, baud);
    cfsetospeed(&options, baud);

    // c_cflag: control mode
    options.c_cflag |= (CLOCAL | CREAD); // enable receiver, ignore modem ctrl
    options.c_cflag &= ~PARENB;          // no parity
    options.c_cflag &= ~CSTOPB;          // 1 stop bit
    options.c_cflag &= ~CSIZE;           // clear data bits
    options.c_cflag |= CS8;              // 8 data bits
    options.c_cflag &= ~CRTSCTS;         // disable HW flow control

    // c_lflag: local mode
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG | IEXTEN);

    // c_iflag: input mode
    options.c_iflag &= ~(IXON | IXOFF | IXANY | ICRNL | INLCR | IGNCR | BRKINT | PARMRK | INPCK | ISTRIP);

    // c_oflag: output mode
    options.c_oflag &= ~OPOST;

    // c_cc: control characters
    options.c_cc[VMIN]  = 1;  // read() returns when 1 byte is available
    options.c_cc[VTIME] = 0;  // no timeout

    // Apply settings
    tcflush(fd, TCIFLUSH); // flush input buffer
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
    std::cout << "Sent to bus: " << message << std::endl;
}

std::pair<MessageType, std::string> PiBus::poll() {
    char buffer[1024] = {0}; // Zero-initialize the buffer
    ssize_t bytes = read(fd, buffer, sizeof(buffer) - 1);
    std::cout << "received: " << buffer << std::endl;
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

std::string PiBus::rawRead() {
    char buffer[1024] = {0}; // Zero-initialize the buffer
    ssize_t bytes = recv(fd, buffer, sizeof(buffer) - 1, 0); // Use recv instead of read
    if (bytes > 0) {
        buffer[bytes] = '\0';
        return std::string(buffer);
    }
    return "";
}