#pragma once
#include <Arduino.h>

/* ---- Wi-Fi / Watchdog / Hostname ---- */
constexpr const char *HOST_NAME = "Generator Mallorca";
constexpr const char *DEFAULT_WIFI_SSID = "Home";
constexpr const char *DEFAULT_WIFI_PASS = "31023102";

constexpr uint8_t ADC_SAMPLES = 10; //
constexpr float CALIBRATE_VOLTAGE_BAT = 0.0010330365683173f;
constexpr float CALIBRATE_VOLTAGE_OUT = 1.0f;

// ZMPT101B (Abdurraziq): Vrms >= порога ⇒ генератор працює
constexpr uint16_t ZMPT_MAINS_HZ = 50; // частота мережі для конструктора
constexpr float ZMPT_SENSITIVITY = 596.0f;
constexpr uint8_t ZMPT_RMS_LOOPS = 3;     // скільки періодів усереднювати (1..N)
constexpr int ZMPT_RMS_THRESHOLD_V = 100; // Vrms > 80 В ⇒ працює

/* ---- Періодики/таймаути ---- */
constexpr int STATE_POLL_PERIOD_MS = 60000;
constexpr int NET_LOSS_AUTO_STOP_MS = 2 * 60 * 60 * 1000;
constexpr int RUNTIME_CHECKPOINT_MS = 1 * 60 * 1000;

// Підтримка АКБ: імпульс на реле кожні 8 год, тривалість 5 с
constexpr uint64_t BAT_KEEPALIVE_PERIOD_MS = 8ULL * 60ULL * 60ULL * 1000ULL;
constexpr uint32_t BAT_KEEPALIVE_PULSE_MS = 5000UL;
