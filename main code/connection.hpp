#ifndef CONNECTION_H
#define CONNECTION_H

#define TINY_GSM_MODEM_SIM800
#define TINY_GSM_RX_BUFFER 1024

#include <Arduino.h>
#include <WiFi.h>
#include <TinyGsmClient.h>
#include <ThingSpeak.h>

// Pin definitions for TTGO T-Call
#define MODEM_RST            5
#define MODEM_PWKEY          4
#define MODEM_POWER_ON       23
#define MODEM_TX             27
#define MODEM_RX             26

// WiFi and GSM configurations
extern const char* ssid;
extern const char* password;
extern const char apn[];
extern const char gprsUser[];
extern const char gprsPass[];

// GSM and WiFi clients
extern WiFiClient wifiClient;
extern TinyGsm modem;
extern TinyGsmClient gprsClient;

// Mode and connection status
extern bool isWiFiMode;
extern bool isConnected;

void setupGSMModule();
void connectWiFi();
void connectGPRS();
void checkModeButtons();
void checkImpulse();
void sendToThingSpeak(float duree); // Update declaration to include parameter


#endif

