#include "connection.hpp"
#include <WiFiManager.h> // Include WiFiManager library
#include <ThingSpeak.h>


// GPRS Configuration
const char apn[] = "internet.ooredoo.tn";
const char gprsUser[] = "";
const char gprsPass[] = "";

// ThingSpeak Configuration
const char* server = "api.thingspeak.com";
const char* apiKey = "LJNXZE2U5EAVYJXV";
unsigned long channelID = 2324487;

// Pin definitions
const int switchPin = 21;
/*const int wifiButtonPin = 22;
const int gprsButtonPin = 15;*/

// Variables for impulses
unsigned long previousImpulseTime = 0;
unsigned long currentImpulseTime = 0;
unsigned long durationBetweenImpulsions = 0;
int impulseCount = 0;
bool impulseDetected = false;

WiFiClient wifiClient;
TinyGsm modem(Serial2);
TinyGsmClient gprsClient(modem);

bool isWiFiMode = true;
bool isConnected = false;

void setupPins() {
    pinMode(switchPin, INPUT_PULLUP);
}

void setupGSMModule() {
    Serial2.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
    pinMode(MODEM_PWKEY, OUTPUT);
    pinMode(MODEM_RST, OUTPUT);
    pinMode(MODEM_POWER_ON, OUTPUT);
    digitalWrite(MODEM_PWKEY, LOW);
    digitalWrite(MODEM_RST, HIGH);
    digitalWrite(MODEM_POWER_ON, HIGH);
    modem.restart();
}

void connectWiFi() {
    WiFiManager wifiManager;
    // Uncomment the following line to reset WiFi settings if needed
    // wifiManager.resetSettings();
    
    // Automatically connect using previously saved credentials or open a portal for configuration
    if (!wifiManager.autoConnect("DataLoger-WiFi")) {
        Serial.println("Failed to connect to WiFi");
        wifiManager.resetSettings();
        ESP.restart();
        Serial.println("WiFi settings reset");
        delay(1000);
        isConnected = false;
    } else {
        Serial.println("Connected to WiFi!");
        ThingSpeak.begin(wifiClient);
        isConnected = true;
        isWiFiMode = true;
    }
}

void connectGPRS() {
    Serial.println("Connecting to GPRS...");
    if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
        Serial.println("GPRS connection failed");
        isConnected = false;
    } else {
        Serial.println("GPRS connected");
        ThingSpeak.begin(gprsClient);
        isConnected = true;
    }
}

void checkImpulse() {
    if (digitalRead(switchPin) == LOW) {
        currentImpulseTime = millis();
        impulseCount++;

        if (impulseDetected) {
            durationBetweenImpulsions = currentImpulseTime - previousImpulseTime;
            Serial.print("Nombre d'impulsions : ");
            Serial.println(impulseCount);
            Serial.print("Durée entre deux impulsions : ");
            Serial.print(durationBetweenImpulsions / 1000.0);
            Serial.println(" secondes");

            // Envoyer la durée à ThingSpeak
            sendToThingSpeak(durationBetweenImpulsions / 1000.0);
        } else {
            Serial.print("Nombre d'impulsions : ");
            Serial.println(impulseCount);
            Serial.println("Première impulsion détectée");
        }

        previousImpulseTime = currentImpulseTime;
        impulseDetected = true;
        delay(100); // Debounce delay
    }
}

void sendToThingSpeak(float duree) {
    Serial.println("Connecting to ThingSpeak...");
    if (isConnected) {
        if (isWiFiMode) {
            if (wifiClient.connect(server, 80)) {
                ThingSpeak.setField(2, duree);
                int responseCode = ThingSpeak.writeFields(channelID, apiKey);
                if (responseCode == 200) {
                    Serial.println("Data sent to ThingSpeak successfully via WiFi");
                } else {
                    Serial.print("Failed to send data to ThingSpeak, HTTP error code: ");
                    Serial.println(responseCode);
                }
                wifiClient.stop();
            } else {
                Serial.println("Connection to ThingSpeak failed");
            }
        } else {
            if (gprsClient.connect(server, 80)) {
                ThingSpeak.setField(2, duree);
                int responseCode = ThingSpeak.writeFields(channelID, apiKey);
                if (responseCode == 200) {
                    Serial.println("Data sent to ThingSpeak successfully via GPRS");
                } else {
                    Serial.print("Failed to send data to ThingSpeak, HTTP error code: ");
                    Serial.println(responseCode);
                }
                gprsClient.stop();
            } else {
                Serial.println("Connection to ThingSpeak failed");
            }
        }
    } else {
        Serial.println("Not connected to network");
    }
}
