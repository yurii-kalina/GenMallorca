#pragma once
#include <Arduino.h>
#include <stdint.h>

constexpr uint8_t RELAY_START_PIN = 26;          // Реле start
constexpr uint8_t RELAY_STOP_PIN = 13;           // Реле stop
constexpr uint8_t ADC_BAT_SDA_PIN = 21;          // ADC — батарея SDA
constexpr uint8_t ADC_BAT_SCL_PIN = 22;          // ADC — батарея SCL
constexpr uint8_t ADC_OUT_VOLTAGE_PIN = 32;      // ADC — вихід генератора
constexpr uint8_t RELAY_ACTIVE_BATTERY_PIN = 27; // ACTIVE батарея
