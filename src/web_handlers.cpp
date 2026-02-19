#include <WebServer.h>
#include <ArduinoJson.h>
#include <cstring>

#include "generator_ops.h"
#include "generator_state.h"
#include "config.h"

extern WebServer server;

static void sendJson(int code, JsonDocument &doc)
{
  String out;
  serializeJson(doc, out);
  server.send(code, "application/json", out);
}

// POST /start
void handleStart()
{
  auto r = startGenerator();

  JsonDocument doc;
  doc["ok"] = r.ok;
  doc["status"] = r.status;
  doc["generatorRun"] = r.generatorRun;

  int code = r.ok ? 200 : (strcmp(r.status, "already_running") == 0 ? 409 : 504);
  sendJson(code, doc);
}

// POST /stop
void handleStop()
{
  auto r = stopGenerator();

  JsonDocument doc;
  doc["ok"] = r.ok;
  doc["status"] = r.status;
  doc["generatorRun"] = r.generatorRun;

  sendJson(r.ok ? 200 : 504, doc);
}

// GET /status
void handleStatus()
{
  auto s = readStatus();

  JsonDocument doc;

  doc["host"] = HOST_NAME;
  doc["generatorRun"] = s.generatorRun;
  doc["analogBattery"] = s.analogBattery;
  doc["voltageBattery"] = s.voltageBattery;
  doc["voltageOutput"] = s.voltageOutput;
  doc["totalRuntimeMs"] = (uint64_t)gTotalRuntimeMs;
  doc["runtimeHours"] = (uint32_t)(gTotalRuntimeMs / 3600000ULL);

  sendJson(200, doc);
}

// GET /uptime
void handleUptime()
{
  JsonDocument doc;
  doc["totalRuntimeMs"] = (uint64_t)gTotalRuntimeMs;
  doc["runtimeHours"] = (uint32_t)(gTotalRuntimeMs / 3600000ULL);

  sendJson(200, doc);
}
