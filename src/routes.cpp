#include "routes.h"
#include "web_handlers.h"

#include <WebServer.h>

extern WebServer server;

void setupRoutes()
{
  server.on("/start", HTTP_POST, handleStart);
  server.on("/stop", HTTP_POST, handleStop);
  server.on("/status", HTTP_GET, handleStatus);
  server.on("/uptime", HTTP_GET, handleUptime);

  server.begin();
}
