#ifndef BUS_H
#define BUS_H

#include <Message.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <cstring>

class Bus {
public:
    Bus();
    ~Bus();
    void openSerial(const char* port, int baud);
    void send(MessageType type, const char* data);
    std::pair<MessageType, std::string> poll();
private:
    int fd;
};

#endif // BUS_H