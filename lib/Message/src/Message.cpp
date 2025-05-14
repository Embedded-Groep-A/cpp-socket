#include "Message.h"


std::string typeToString(MessageType type) {
    switch (type) {
        case MessageType::RGB: return "RGB";
        case MessageType::ACCEPT: return "ACCEPT";
        case MessageType::REJECT: return "REJECT";
        case MessageType::OPEN: return "OPEN";
        case MessageType::BEL: return "BEL";
        case MessageType::UID: return "UID";
        case MessageType::BRAND: return "BRAND";
        case MessageType::STATE: return "STATE";
        case MessageType::TEXT: return "TEXT";
        default: return "UNKNOWN";
    }
}

MessageType stringToType(const std::string& str) {
    if (str == "RGB") return MessageType::RGB;
    if (str == "ACCEPT") return MessageType::ACCEPT;
    if (str == "REJECT") return MessageType::REJECT;
    if (str == "OPEN") return MessageType::OPEN;
    if (str == "BEL") return MessageType::BEL;
    if (str == "UID") return MessageType::UID;
    if (str == "BRAND") return MessageType::BRAND;
    if (str == "STATE") return MessageType::STATE;
    if (str == "TEXT") return MessageType::TEXT;
    return MessageType::UNKNOWN;
}