#include "adc_utils.h"
#include <Wire.h>
#include <Adafruit_ADS1X15.h>

static Adafruit_ADS1115 ads;
static bool adsInited = false;

bool initAds1115()
{
    if (adsInited)
        return true;

    Wire.begin(ADC_BAT_SDA_PIN, ADC_BAT_SCL_PIN);
    Wire.setClock(100000);

    if (!ads.begin())
    {
        adsInited = false;
        return false;
    }

    ads.setGain(GAIN_TWOTHIRDS);
    adsInited = true;
    return true;
}

VoltReading readVoltage()
{
    if (!adsInited && !initAds1115())
    {
        return {0, NAN};
    }

    int16_t raw = ads.readADC_SingleEnded(0);
    float volts = raw * CALIBRATE_VOLTAGE_BAT;

    return {raw, volts};
}

void batteryKeepAliveTick()
{
    static unsigned long lastPulseMs = 0;
    static unsigned long pulseStartMs = 0;
    static bool pulseActive = false;

    const unsigned long now = millis();

    // Якщо імпульс активний — чекаємо BAT_KEEPALIVE_PULSE_MS і відпускаємо реле
    if (pulseActive)
    {
        if ((unsigned long)(now - pulseStartMs) >= (unsigned long)BAT_KEEPALIVE_PULSE_MS)
        {
            digitalWrite(RELAY_ACTIVE_BATTERY_PIN, HIGH); // нормальний стан
            pulseActive = false;
            lastPulseMs = now;
        }
        return;
    }

    // Перший запуск — ініціалізація таймера
    if (lastPulseMs == 0)
    {
        lastPulseMs = now;
        return;
    }

    // Кожні BAT_KEEPALIVE_PERIOD_MS — імпульс
    if ((uint64_t)(now - lastPulseMs) >= BAT_KEEPALIVE_PERIOD_MS)
    {
        digitalWrite(RELAY_ACTIVE_BATTERY_PIN, LOW); // спрацювання реле
        pulseStartMs = now;
        pulseActive = true;
    }
}
