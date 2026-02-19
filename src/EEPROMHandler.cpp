#include "EEPROMHandler.h"
#include <EEPROM.h>

void initEEPROM()
{
    EEPROM.begin(EEPROM_SIZE);
}

void saveEepromTotalRuntimeMs(uint64_t ms)
{
    // 8 байт, big-endian
    for (int i = 0; i < 8; ++i)
    {
        EEPROM.write(EEPROM_TOTAL_RUNTIME_MS_ADDR + i,
                     (uint8_t)((ms >> (56 - i * 8)) & 0xFF));
    }
    EEPROM.commit();
}

uint64_t loadEepromTotalRuntimeMs()
{
    uint64_t ms = 0;
    for (int i = 0; i < 8; ++i)
    {
        ms = (ms << 8) | (uint8_t)EEPROM.read(EEPROM_TOTAL_RUNTIME_MS_ADDR + i);
    }

    if (ms == 0xFFFFFFFFFFFFFFFFULL)
        return 0ULL;
    return ms;
}
