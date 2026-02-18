#include <ArduinoJson.h>
#include "web_handlers.h"
#include "http_json.h"
#include <cstring>
#include "generator/generator_ops.h"
#include "services/LogSender.h"
#include "generator/generator_state.h"
#include "config/config.h"

// POST /start
void handleStart()
{
  const auto r = startGenerator();
  sendLogToServer(r.ok ? "info" : "error", r.status);

  sendJson(r.ok ? 200 : (strcmp(r.status, "already_running") == 0 ? 409 : 504),
           [&](JsonDocument &d)
           {
             d["ok"] = r.ok;
             d["status"] = r.status;
             d["generatorRun"] = r.generatorRun;
           });
}

// POST /stop
void handleStop()
{
  const auto r = stopGenerator();
  sendLogToServer(r.ok ? "info" : "error", r.status);

  sendJson(r.ok ? 200 : 504, [&](JsonDocument &d)
           {
    d["status"] = r.status;
    d["generatorRun"] = r.generatorRun;
    d["stopAttempt"] = r.stopAttempt; });
}

// GET /status
void handleStatus()
{
  const auto s = readStatus();
  sendJson(200, [&](JsonDocument &d)
           {
    d["host"] = HOST_NAME;
    d["generatorRun"] = s.generatorRun;
    d["analogBattery"] = s.analogBattery;
    d["voltageBattery"] = s.voltageBattery;
    d["voltageOutput"]    = s.voltageOutput;
    d["runtimeHours"] = (uint32_t)(gTotalRuntimeMs / 3600000ULL); });
}

// GET /uptime
void handleUptime()
{
  sendJson(200, [&](JsonDocument &d)
           { d["runtimeHours"] = (uint32_t)(gTotalRuntimeMs); });
}
