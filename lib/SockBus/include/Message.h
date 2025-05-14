#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <cstdint>

enum class MessageType : uint8_t {
    UNKNOWN = 0, //VOOR ONBEKENDE TYPEDATA
    RGB = 1, //RGB VALUES VOOR LAMP
    ACCEPT = 2, // SIGNAAL NAAR LICHTKRANT MET USER
    REJECT = 3, // SIGNAAL NAAR LICHTKRANT EN BALIE
    OPEN = 4, // SIGNAAL NAAR DEUR 
    BEL = 5, // SIGNAAL NAAR BALIE
    UID = 6, // SIGNAAL NAAR PI MET UID
    BRAND = 7, // SIGNAAL VOOR DEUREN EN LAMPEN ALS BRAND
    STATE = 8, // ON/OFF SIGNAAL VOOR LAMPEN EN STOEL
    TEXT = 9, // TEXT STUREN [TESTING]
};

struct ClientMessage {
    std::string clientID;
    MessageType type;
    std::string message;
};

std::string typeToString(MessageType type);
MessageType stringToType(const std::string& str);

#endif // MESSAGE_H