#pragma once
#include <stdint.h>
#include <stddef.h>

#define EEPROM_SIZE                  192

#define EEPROM_IP_ADDR               0
#define EEPROM_PORT_ADDR             16
#define EEPROM_WIFI_SSID_ADDR        32
#define EEPROM_WIFI_PASS_ADDR        96
#define EEPROM_TOTAL_RUNTIME_MS_ADDR 160

void initEEPROM();

void saveEepromBackEndServer(const char* ip, uint16_t port);
bool loadEepromBackEndServerURL(char* ipBuffer, size_t bufSize, uint16_t* port);

void saveEepromWiFiCredentials(const char* ssid, const char* password);
bool loadEepromWiFiCredentials(char* ssidBuffer, size_t ssidBufSize, char* passBuffer, size_t passBufSize);

void     saveEepromTotalRuntimeMs(uint64_t ms);
uint64_t loadEepromTotalRuntimeMs();
