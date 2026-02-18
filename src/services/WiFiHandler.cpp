#include <WiFi.h>
#include <esp_wifi.h>

#include "WiFiHandler.h"
#include "config/config.h"
#include "EEPROMHandler.h"
#include "LogSender.h"
#include "api/web_handlers.h"

#include <ArduinoJson.h>

char ssid[64];
char password[64];

static unsigned long nextReconnectAt = 0;
static uint32_t backoffMs = 1000;

static void startWiFiConnect()
{
  WiFi.begin(ssid, password);
}

static void onWiFiEvent(WiFiEvent_t event)
{
  switch (event)
  {
  case ARDUINO_EVENT_WIFI_STA_GOT_IP:
    backoffMs = 1000;
    break;

  case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
    nextReconnectAt = millis() + backoffMs;
    backoffMs = min<uint32_t>(backoffMs * 2, 60000);
    break;

  default:
    break;
  }
}

void initWiFi()
{
  bool valid = loadEepromWiFiCredentials(ssid, sizeof(ssid), password, sizeof(password));
  if (!valid)
  {
    strncpy(ssid, DEFAULT_WIFI_SSID, sizeof(ssid));
    strncpy(password, DEFAULT_WIFI_PASS, sizeof(password));
    ssid[sizeof(ssid) - 1] = 0;
    password[sizeof(password) - 1] = 0;
    saveEepromWiFiCredentials(ssid, password);
  }

  WiFi.setHostname(HOST_NAME);
  WiFi.mode(WIFI_STA);
  WiFi.persistent(false);
  WiFi.setAutoReconnect(true);
  WiFi.onEvent(onWiFiEvent);

  startWiFiConnect();
}

void handleWiFiReconnect()
{
  if (WiFi.status() != WL_CONNECTED && millis() >= nextReconnectAt)
  {
    WiFi.disconnect(false, false);
    delay(10);
    startWiFiConnect();

    nextReconnectAt = millis() + backoffMs;
    backoffMs = min<uint32_t>(backoffMs * 2, 60000);
  }
}

void getWiFiCredentials()
{
  char eSsid[64] = {0}, ePass[64] = {0};
  loadEepromWiFiCredentials(eSsid, sizeof(eSsid), ePass, sizeof(ePass));

  auto mask = [](const char *s)
  { return String(s).isEmpty() ? "" : "********"; };

  JsonDocument doc;
  doc["current"]["ssid"] = ssid;
  doc["current"]["password"] = mask(password);
  doc["eeprom"]["ssid"] = eSsid;
  doc["eeprom"]["password"] = mask(ePass);
  doc["wifi"]["connected"] = (WiFi.status() == WL_CONNECTED);
  doc["wifi"]["ip"] = WiFi.localIP().toString();

  String resp;
  serializeJson(doc, resp);
  server.send(200, "application/json", resp);
}

void setWiFiCredentials()
{
  if (!server.hasArg("plain"))
  {
    server.send(400, "application/json", "{\"error\":\"Missing JSON\"}");
    return;
  }

  JsonDocument doc;
  if (deserializeJson(doc, server.arg("plain")))
  {
    server.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
    return;
  }

  const char *newSsid = doc["ssid"];
  const char *newPass = doc["password"];
  if (!newSsid || !newPass || !strlen(newSsid) || !strlen(newPass))
  {
    server.send(400, "application/json", "{\"error\":\"Empty creds\"}");
    return;
  }

  saveEepromWiFiCredentials(newSsid, newPass);

  strncpy(ssid, newSsid, sizeof(ssid));
  strncpy(password, newPass, sizeof(password));
  ssid[sizeof(ssid) - 1] = 0;
  password[sizeof(password) - 1] = 0;

  WiFi.disconnect(false, false);
  delay(50);

  backoffMs = 1000;
  nextReconnectAt = millis() + 100;

  startWiFiConnect();

  server.send(200, "application/json", "{\"status\":\"accepted\"}");
}
