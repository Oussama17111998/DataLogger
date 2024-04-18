const int switchPin = 34;  // Broche à laquelle le switch est connecté
int impulsion = 0;     // Compteur d'impulsions
int compteur = 0;
bool start = false;
const int INTERVAL = 2;
// Your GPRS credentials (leave empty, if not needed)
const char apn[]      = "internet.ooredoo.tn"; // APN (example: internet.vodafone.pt) use https://wiki.apnchanger.org
const char gprsUser[] = ""; // GPRS User
const char gprsPass[] = ""; // GPRS Password

// SIM card PIN (leave empty, if not defined)
const char simPIN[]   = ""; 

// TTGO T-Call pins
#define MODEM_RST            5
#define MODEM_PWKEY          4
#define MODEM_POWER_ON       23
#define MODEM_TX             27
#define MODEM_RX             26
#define I2C_SDA              21
#define I2C_SCL              22

#include <HardwareSerial.h>

// Configure TinyGSM library
#define TINY_GSM_MODEM_SIM800      // Modem is SIM800
#define TINY_GSM_RX_BUFFER   1024  // Set RX buffer to 1Kb

// Define the serial console for debug prints, if needed
//#define DUMP_AT_COMMANDS

#include <Wire.h>
#include <TinyGsmClient.h>

TinyGsm modem(Serial2);
// I2C for SIM800 (to keep it running when powered from battery)
TwoWire I2CPower = TwoWire(0);

// TinyGSM Client for Internet connection
TinyGsmClient client(modem);

void verifConnexion() {
    // Unlock your SIM card with a PIN if needed
  if (strlen(simPIN) && modem.getSimStatus() != 3 ) {
    modem.simUnlock(simPIN);
  }
   Serial.print("Connecting to APN: ");
  Serial.print(apn);


  if (modem.gprsConnect(apn, gprsUser, gprsPass)) {
    Serial.print("SIM Successfully connected ");
    Serial.println("Server connected ");
    Serial.println("GRPS connected");
  }
  else {
    Serial.println("Fail to connect to GRPS");
    Serial.println("Fail to connect to server");
  }

}

void setup() {
  // Set serial monitor debugging window baud rate to 115200
  Serial.begin(115200);

  // Start I2C communication
  I2CPower.begin(I2C_SDA, I2C_SCL, 400000);
  
  // Set modem reset, enable, power pins
  pinMode(MODEM_PWKEY, OUTPUT);
  pinMode(MODEM_RST, OUTPUT);
  pinMode(MODEM_POWER_ON, OUTPUT);
  pinMode(switchPin, INPUT_PULLUP);  // Configuration de la broche en entrée avec une résistance de pull-up activée
  digitalWrite(MODEM_PWKEY, LOW);
  digitalWrite(MODEM_RST, HIGH);
  digitalWrite(MODEM_POWER_ON, HIGH);

  // Set GSM module baud rate and UART pins
  Serial2.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
  delay(3000);

  // Restart SIM800 module, it takes quite some time
  // To skip it, call init() instead of restart()
  Serial.println("Initializing modem...");
  modem.restart();
  // use modem.init() if you don't need the complete restart

  // Unlock your SIM card with a PIN if needed
  if (strlen(simPIN) && modem.getSimStatus() != 3 ) {
    modem.simUnlock(simPIN);
  }
   Serial.print("Connecting to APN: ");
  Serial.print(apn);


  if (modem.gprsConnect(apn, gprsUser, gprsPass)) {
    Serial.print("SIM Successfully connected ");
    Serial.println("Server connected ");
    Serial.println("GRPS connected");
  }
  else {
    Serial.println("Fail to connect to GRPS");
    Serial.println("Fail to connect to server");
  }
}

void loop() {
  if (digitalRead(switchPin) == HIGH) {
    impulsion++;
    Serial.print("Nombre d'impulsions : ");
    Serial.println(impulsion);  
    unsigned long nextReading = millis() + INTERVAL * 1000;
     while (millis() < nextReading)
      {
        delay(1);
      } 
}
}

