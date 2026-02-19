#pragma once
#include <Arduino.h>
#include "pins.h"
#include "config.h"

struct VoltReading
{
    int16_t raw;
    float volts;
};

bool initAds1115();

VoltReading readVoltage();
void batteryKeepAliveTick();
