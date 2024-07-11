#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "main.hpp"

void setupPins();
void setupGSMModule();
void connectWiFi();
void connectGPRS();
void checkModeButtons();
void checkImpulse();

#endif // CONNECTION_HPP
