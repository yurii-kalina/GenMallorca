#include "WiFiHandler.h"
#include "config.h"

static unsigned long previousMillis = 0;
static const unsigned long interval = 10000;

void initWiFi()
{
  WiFi.setHostname(HOST_NAME);
  WiFi.mode(WIFI_STA);
  WiFi.persistent(false);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
}

void handleWiFiReconnect()
{
  unsigned long currentMillis = millis();

  // Якщо WiFi відсутній — пробуємо перепідключитись
  if ((WiFi.status() != WL_CONNECTED) &&
      (currentMillis - previousMillis >= interval))
  {
    Serial.print(currentMillis);
    Serial.println(" Reconnecting to WiFi...");

    WiFi.disconnect();
    WiFi.reconnect();

    previousMillis = currentMillis;
  }
}
