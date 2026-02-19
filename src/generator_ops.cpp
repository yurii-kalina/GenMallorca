#include "generator_ops.h"
#include <Arduino.h>
#include "config.h"
#include "pins.h"
#include "generator_state.h"
#include "adc_utils.h"

namespace
{
  inline void pulseRelay(uint8_t pin, uint32_t pulseMs)
  {
    digitalWrite(pin, LOW);
    delay(pulseMs);
    digitalWrite(pin, HIGH);
  }

  inline bool waitForState(bool wantRun, uint32_t timeoutMs)
  {
    const unsigned long started = millis();
    while ((uint32_t)(millis() - started) < timeoutMs)
    {
      const bool run = pollGeneratorRun(true);
      if (run == wantRun)
        return true;
      delay(100);
      yield();
    }
    return false;
  }
}

// ---- START ----
StartResult startGenerator()
{
  StartResult r{false, generatorRun, "already_running"};
  if (generatorRun)
  {
    return r;
  }

  pulseRelay(RELAY_START_PIN, START_PULSE_MS);
  delay(START_STABILIZE_MS);

  const bool ok = waitForState(true, START_TIMEOUT_MS);
  r.generatorRun = generatorRun;
  r.ok = ok;
  r.status = ok ? "started" : "start_timeout";
  return r;
}

// ---- STOP ----
StopResult stopGenerator()
{
  StopResult r{false, generatorRun, "stop_failed"};

  pulseRelay(RELAY_STOP_PIN, STOP_PULSE_MS);
  delay(STOP_STABILIZE_MS);

  const bool ok = waitForState(false, STOP_TIMEOUT_MS);

  r.generatorRun = generatorRun;
  r.ok = ok;
  r.status = ok ? "stopped" : "stop_failed";
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
