// main.cpp
#include <ElegantOTA.h>
#include "config/pins.h"
#include "generator/generator_state.h"
#include "services/WiFiHandler.h"
#include "services/BackEndServer.h"
#include "api/web_handlers.h"
#include "drivers/adc_utils.h"
#include <WiFi.h>
#include "drivers/adc_utils.h"
#include "services/EEPROMHandler.h"
#include "api/routes.h"
#include <esp_system.h>
#include <esp_task_wdt.h>

static constexpr uint32_t WDT_TIMEOUT_S = 180;

void setup()
{
    Serial.begin(115200);
    delay(200);
    Serial.printf("[BOOT] reset_reason=%d\n", (int)esp_reset_reason());

    // Watchdog: перезавантажить плату, якщо loop/task зависне
    esp_task_wdt_init(WDT_TIMEOUT_S, true);
    esp_task_wdt_add(NULL);

    pinMode(RELAY_START_PIN, OUTPUT);
    pinMode(RELAY_STOP_PIN, OUTPUT);
    pinMode(RELAY_ACTIVE_BATTERY_PIN, OUTPUT);

    digitalWrite(RELAY_START_PIN, HIGH);
    digitalWrite(RELAY_STOP_PIN, HIGH);
    digitalWrite(RELAY_ACTIVE_BATTERY_PIN, HIGH);

    pinMode(ADC_OUT_VOLTAGE_PIN, INPUT);

    initEEPROM();
    initWiFi();
    initBackEndServerFromEEPROM();
    initGeneratorState();

    ElegantOTA.begin(&server);
    setupRoutes();
}

void loop()
{

    esp_task_wdt_reset();

    static uint32_t wifiLostSince = 0;
    if (WiFi.status() != WL_CONNECTED)
    {
        if (wifiLostSince == 0)
            wifiLostSince = millis();
        if (millis() - wifiLostSince > 300000UL)
        {
            Serial.println("[WIFI] offline too long -> ESP.restart()");
            delay(50);
            ESP.restart();
        }
    }
    else
    {
        wifiLostSince = 0;
    }

    server.handleClient();
    ElegantOTA.loop();
    handleWiFiReconnect();
    updateGeneratorState();
    batteryKeepAliveTick();
}
