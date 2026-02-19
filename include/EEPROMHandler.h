#pragma once
#include <stdint.h>

#define EEPROM_SIZE 192
#define EEPROM_TOTAL_RUNTIME_MS_ADDR 160

void initEEPROM();

void saveEepromTotalRuntimeMs(uint64_t ms);
uint64_t loadEepromTotalRuntimeMs();
