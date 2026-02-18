#include "EEPROMHandler.h"
#include <EEPROM.h>
#include <Arduino.h>

static bool isValidEEPROMString(const char* buf) {
    return !(buf[0] == '\0' || (uint8_t)buf[0] == 0xFF || strlen(buf) == 0);
}

void initEEPROM() {
    EEPROM.begin(EEPROM_SIZE);
}

// -------------------------- Backend server --------------------------
void saveEepromBackEndServer(const char* ip, uint16_t port) {
    for (int i = 0; i < 16; ++i) {
        EEPROM.write(EEPROM_IP_ADDR + i, ip[i]);
        if (ip[i] == '\0') break;
    }
    EEPROM.write(EEPROM_PORT_ADDR,     (port >> 8) & 0xFF);
    EEPROM.write(EEPROM_PORT_ADDR + 1, port & 0xFF);
    EEPROM.commit();
}

bool loadEepromBackEndServerURL(char* ipBuffer, size_t bufSize, uint16_t* port) {
    for (int i = 0; i < (int)(bufSize - 1); ++i) {
        ipBuffer[i] = EEPROM.read(EEPROM_IP_ADDR + i);
        if (ipBuffer[i] == '\0' || (uint8_t)ipBuffer[i] == 0xFF) break;
    }
    ipBuffer[bufSize - 1] = '\0';

    *port = (EEPROM.read(EEPROM_PORT_ADDR) << 8) | EEPROM.read(EEPROM_PORT_ADDR + 1);
    return isValidEEPROMString(ipBuffer);
}

// -------------------------- Wi-Fi creds --------------------------
void saveEepromWiFiCredentials(const char* ssid, const char* password) {
    for (int i = 0; i < 64; ++i) {
        EEPROM.write(EEPROM_WIFI_SSID_ADDR + i, ssid[i]);
        if (ssid[i] == '\0') break;
    }
    for (int i = 0; i < 64; ++i) {
        EEPROM.write(EEPROM_WIFI_PASS_ADDR + i, password[i]);
        if (password[i] == '\0') break;
    }
    EEPROM.commit();
}

bool loadEepromWiFiCredentials(char* ssidBuffer, size_t ssidBufSize, char* passBuffer, size_t passBufSize) {
    for (int i = 0; i < (int)(ssidBufSize - 1); ++i) {
        ssidBuffer[i] = EEPROM.read(EEPROM_WIFI_SSID_ADDR + i);
        if (ssidBuffer[i] == '\0' || (uint8_t)ssidBuffer[i] == 0xFF) break;
    }
    ssidBuffer[ssidBufSize - 1] = '\0';

    for (int i = 0; i < (int)(passBufSize - 1); ++i) {
        passBuffer[i] = EEPROM.read(EEPROM_WIFI_PASS_ADDR + i);
        if (passBuffer[i] == '\0' || (uint8_t)passBuffer[i] == 0xFF) break;
    }
    passBuffer[passBufSize - 1] = '\0';

    return isValidEEPROMString(ssidBuffer) && isValidEEPROMString(passBuffer);
}

// -------------------------- Total Runtime (мс, 8 байт) --------------------------
void saveEepromTotalRuntimeMs(uint64_t ms) {
    for (int i = 0; i < 8; ++i) {
        EEPROM.write(EEPROM_TOTAL_RUNTIME_MS_ADDR + i, (uint8_t)((ms >> (8 * i)) & 0xFF));
    }
    EEPROM.commit();
}

uint64_t loadEepromTotalRuntimeMs() {
    uint64_t ms = 0;
    for (int i = 0; i < 8; ++i) {
        ms |= (uint64_t)EEPROM.read(EEPROM_TOTAL_RUNTIME_MS_ADDR + i) << (8 * i);
    }
    if (ms == 0xFFFFFFFFFFFFFFFFULL) return 0ULL;
    return ms;
}
