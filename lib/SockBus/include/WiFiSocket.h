#ifndef WIFISOCKET_H
#define WIFISOCKET_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "Message.h"

class WiFiSocket {
public:
    WiFiSocket(const char* ssid, const char* password);
    ~WiFiSocket();
    void connectSocket(const char* host, uint16_t port, cont char* id);
    void disconnectSocket();
    void sendToServer(MessageType type, const String& message);
    std::pair<MessageType, String> pollServer();

private:
    WiFiClient client;
    String serverIP;
    uint16_t serverPort;
    

#endif // WIFISOCKET_H