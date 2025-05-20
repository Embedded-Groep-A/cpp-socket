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
    char buffer[1024];
    ssize_t bytes = read(fd, buffer, sizeof(buffer));
    if (bytes > 0) {
        inputBuffer.append(buffer, bytes);

        size_t newlinePos;
        // Process each full line (ending with '\n')
        while ((newlinePos = inputBuffer.find('\n')) != std::string::npos) {
            std::string line = inputBuffer.substr(0, newlinePos);
            inputBuffer.erase(0, newlinePos + 1);

            std::cout << "received: " << line << std::endl;

            size_t start = line.find('[');
            size_t end = line.find(']');
            if (start != std::string::npos && end != std::string::npos && end > start + 1) {
                std::string typeStr = line.substr(start + 1, end - start - 1);
                MessageType type = stringToType(typeStr);

                std::string data;
                if (end + 1 < line.size()) {
                    data = line.substr(end + 1);
                    if (!data.empty() && data[0] == ' ') data = data.substr(1);
                }

                std::cout << "Parsed message type: " << typeStr << ", data: " << data << std::endl;
                return {type, data};
            } else {
                std::cerr << "Malformed message line: " << line << std::endl;
            }
        }
    }

    return {MessageType::UNKNOWN, ""};
}



