#include "routes.h"
#include "api/web_handlers.h"
#include "services/BackEndServer.h"
#include "services/WiFiHandler.h"

void setupRoutes() {
  server.on("/start",  HTTP_POST, handleStart);
  server.on("/stop",   HTTP_POST, handleStop);
  server.on("/status", HTTP_GET,  handleStatus);
  server.on("/uptime", HTTP_GET,  handleUptime);

  server.on("/backend/url", HTTP_GET,  getBackEndServerURL);
  server.on("/backend/url", HTTP_POST, setBackEndServerURL);
  server.on("/wifi",        HTTP_GET,  getWiFiCredentials);
  server.on("/wifi",        HTTP_POST, setWiFiCredentials);

  server.begin();
}
