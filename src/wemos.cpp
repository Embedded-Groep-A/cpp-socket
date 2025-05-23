#include "WifiSocket.h"
#include <arduino.h>
#include <ESP8266WiFi.h>

const char *ssid = "707";
const char *password = "DEFCAB1960";
const char *host = "bramsvoorhoofd.local";
int port = 8181;

WiFiSocket wifiSocket;

void setup() {
    Serial.begin(115200);
    wifiSocket.connectWiFi(ssid, password);
    wifiSocket.connectSocket(host, port, "WEMOS");
    delay(1000);
}

void loop() {
    auto [type, message] = wifiSocket.pollServer();
    wifiSocket.sendToServer(MessageType::STATE, "OFF");
    delay(1000);
}