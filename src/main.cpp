#include <ElegantOTA.h>
#include <WiFi.h>
#include <WebServer.h>
#include "pins.h"
#include "generator_state.h"
#include "WiFiHandler.h"
#include "adc_utils.h"
#include "EEPROMHandler.h"
#include "routes.h"

WebServer server(80);

void setup()
{
    Serial.begin(115200);
    delay(200);

    pinMode(RELAY_START_PIN, OUTPUT);
    pinMode(RELAY_STOP_PIN, OUTPUT);
    pinMode(RELAY_ACTIVE_BATTERY_PIN, OUTPUT);

    digitalWrite(RELAY_START_PIN, HIGH);
    digitalWrite(RELAY_STOP_PIN, HIGH);
    digitalWrite(RELAY_ACTIVE_BATTERY_PIN, HIGH);

    pinMode(ADC_OUT_VOLTAGE_PIN, INPUT);

    initEEPROM();
    initWiFi();
    (void)initAds1115();
    initGeneratorState();

    ElegantOTA.begin(&server);
    setupRoutes();
}

void loop()
{
    server.handleClient();
    ElegantOTA.loop();
    handleWiFiReconnect();
    updateGeneratorState();
    batteryKeepAliveTick();
}
