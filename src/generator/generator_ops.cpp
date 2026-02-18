#include "generator_ops.h"
#include <Arduino.h>
#include "config/config.h"
#include "config/pins.h"
#include "services/EEPROMHandler.h"
#include "generator/generator_state.h"
#include "drivers/adc_utils.h"
#include "services/LogSender.h"

// ---- START ----
StartResult startGenerator()
{
  StartResult r{false, generatorRun, "already_running"};
  if (generatorRun)
  {
    return r;
  }

  digitalWrite(RELAY_START_PIN, LOW);
  delay(500);
  digitalWrite(RELAY_START_PIN, HIGH);

  delay(6000);
  bool ok = false;
  if (pollGeneratorRun(true))
  {
    ok = true;
  }
  r.generatorRun = generatorRun;
  r.ok = ok;
  r.status = ok ? "started" : "start_timeout";
  return r;
}

// ---- STOP ----
StopResult stopGenerator()
{
  StopResult r{false, generatorRun, "stop_failed", 0};

  digitalWrite(RELAY_STOP_PIN, LOW);
  delay(1000);
  digitalWrite(RELAY_STOP_PIN, HIGH);
  delay(5000);

  bool stopped = false;
  if (!pollGeneratorRun(true))
  {
    stopped = true;
  }

  r.generatorRun = generatorRun;
  r.ok = !generatorRun;
  r.status = r.ok ? "stopped" : "stop_failed";
  return r;
}

// ---- STATUS ----
StatusData readStatus()
{
  StatusData s{};
  s.generatorRun = pollGeneratorRun(true);

  const VoltReading bat = readVoltage();
  s.analogBattery = bat.raw;
  s.voltageBattery = bat.volts;
  s.voltageOutput = getOutputVrms();
  return s;
}
