#ifndef MAIN_HPP
#define MAIN_HPP

#define TINY_GSM_MODEM_SIM800
#define TINY_GSM_RX_BUFFER 1024

#include <WiFi.h>
#include <TinyGsmClient.h>
#include <ThingSpeak.h>

// WiFi Configuration
extern const char* ssid;
extern const char* password;

// GPRS Configuration
extern const char apn[];
extern const char gprsUser[];
extern const char gprsPass[];

// ThingSpeak Configuration
extern const char* server;
extern const char* apiKey;
extern unsigned long channelID;

// Pin definitions
extern const int switchPin;
extern const int wifiButtonPin;
extern const int gprsButtonPin;

// TTGO T-Call pins
#define MODEM_RST            5
#define MODEM_PWKEY          4
#define MODEM_POWER_ON       23
#define MODEM_TX             27
#define MODEM_RX             26

// Variables for impulses
extern unsigned long previousImpulseTime;
extern unsigned long currentImpulseTime;
extern unsigned long durationBetweenImpulsions;
extern int impulseCount;
extern bool impulseDetected;

extern WiFiClient wifiClient;
extern TinyGsm modem;
extern TinyGsmClient gprsClient;

extern bool isWiFiMode;
extern bool isConnected;

void sendToThingSpeak(float duration);

#endif // MAIN_HPP
