#pragma once
#include <WiFiUdp.h>

extern IPAddress targetIP;
extern uint16_t targetPort;

void initBackEndServerFromEEPROM();
void setBackEndServerURL();
void getBackEndServerURL();
