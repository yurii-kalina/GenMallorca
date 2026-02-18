#pragma once
#include <Arduino.h>

// Результати бізнес-операцій (без HTTP-специфіки)
struct StartResult
{
  bool ok;
  bool generatorRun;
  const char *status; // "started" | "already_running" | "start_timeout"
};

struct StopResult
{
  bool ok;
  bool generatorRun;
  const char *status; // "stopped" | "stop_failed"
  int stopAttempt;
};

struct StatusData
{
  bool generatorRun;
  float analogBattery;
  float voltageBattery;
  float voltageOutput;
  bool powerOn;
};

StartResult startGenerator();
StopResult stopGenerator();
StatusData readStatus();
