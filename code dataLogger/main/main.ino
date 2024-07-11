#include "main.hpp"
#include "connection.hpp"

void setup() {
    Serial.begin(115200);
    setupPins();
    setupGSMModule();
    connectWiFi();
}

void loop() {
    checkModeButtons();
    checkImpulse();
}
