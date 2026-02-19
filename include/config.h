#pragma once
#include <Arduino.h>

/* ---- Wi-Fi / Hostname ---- */
constexpr const char *HOST_NAME = "Gnerator_Malorca_7000";
constexpr const char *WIFI_SSID = "Home";
constexpr const char *WIFI_PASS = "31023102";

constexpr uint8_t ADC_SAMPLES = 10;
constexpr float CALIBRATE_VOLTAGE_BAT = 0.0010330365683173f;

// ZMPT101B (Abdurraziq): Vrms >= порога ⇒ генератор працює
constexpr uint16_t ZMPT_MAINS_HZ = 50; // частота мережі для конструктора
constexpr float ZMPT_SENSITIVITY = 596.0f;
constexpr uint8_t ZMPT_RMS_LOOPS = 3;     // скільки періодів усереднювати (1..N)
constexpr int ZMPT_RMS_THRESHOLD_V = 100; // Vrms > 80 В ⇒ працює

/* ---- Періодики/таймаути ---- */
constexpr int STATE_POLL_PERIOD_MS = 60000;
constexpr int NET_LOSS_AUTO_STOP_MS = 2 * 60 * 60 * 1000;

// Параметри керування реле старт/стоп
constexpr uint32_t START_PULSE_MS = 500;
constexpr uint32_t STOP_PULSE_MS = 1000;
constexpr uint32_t START_STABILIZE_MS = 6000; // пауза після старт-імпульсу
constexpr uint32_t STOP_STABILIZE_MS = 5000;  // пауза після стоп-імпульсу
constexpr uint32_t START_TIMEOUT_MS = 30000;  // очікування "генератор працює"
constexpr uint32_t STOP_TIMEOUT_MS = 30000;   // очікування "генератор зупинився"

// Підтримка АКБ: імпульс на реле кожні 6 год, тривалість 5 с
constexpr uint64_t BAT_KEEPALIVE_PERIOD_MS = 6ULL * 60ULL * 60ULL * 1000ULL;
constexpr uint32_t BAT_KEEPALIVE_PULSE_MS = 5000UL;
