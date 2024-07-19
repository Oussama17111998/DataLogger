#include "main.hpp"
#include "connection.hpp"
#include <WiFiManager.h> // Include WiFiManager library

void setup() {
    Serial.begin(115200);
    setupGSMModule();

    // Try to connect to WiFi using WiFiManager
    connectWiFi();
    
    if (!isConnected) {
        connectGPRS();
    }
}
void loop() {
    if (!isConnected) {
        if (isWiFiMode) {
            connectWiFi();
        }
        if (!isConnected) {
            connectGPRS();
        }
    } else if (isWiFiMode) {
        // Disconnect GPRS if WiFi connects
        modem.gprsDisconnect();
    }
    delay(100); // Add a delay to prevent rapid reconnection attempts
    checkImpulse();
}