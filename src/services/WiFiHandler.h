#pragma once
#include <WiFi.h>

extern char ssid[64];
extern char password[64];

void initWiFi();
void handleWiFiReconnect();
void getWiFiCredentials();
void setWiFiCredentials();
