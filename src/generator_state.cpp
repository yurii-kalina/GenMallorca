#include <WiFi.h>
#include <math.h>
#include <ZMPT101B.h>

#include "generator_state.h"
#include "config.h"
#include "pins.h"
#include "EEPROMHandler.h"

// Уникаємо залежності від HTTP-рівня: автостоп викликає бізнес-операцію.
#include "generator_ops.h"

static ZMPT101B sensor220(ADC_OUT_VOLTAGE_PIN, ZMPT_MAINS_HZ);

// -----------------------------------------------------------------------------
// Стан/лічильники (без проміжних накопичень часу)
// -----------------------------------------------------------------------------
bool generatorRun = false;              // публічний прапорець "генератор працює"
static bool lastGeneratorRun = false;   // попередній стан для детекції фронтів
static unsigned long lastRunCheck = 0;  // останній момент опитування стану
unsigned long generatorStartMillis = 0; // ВСТАНОВЛЮЄТЬСЯ ЛИШЕ У startGenerator()
static unsigned long noInternetSince = 0;
uint64_t gTotalRuntimeMs = 0ULL;

namespace
{
  inline bool elapsed(unsigned long since, unsigned long period)
  {
    return static_cast<long>(millis() - since) >= static_cast<long>(period);
  }

  inline float safeGetRmsVoltage(uint16_t loops)
  {
    float v = sensor220.getRmsVoltage(loops);
    if (isnan(v) || isinf(v) || v < 0.0f)
      return 0.0f;
    return v;
  }
}

void initGeneratorState()
{
  lastRunCheck = 0;
  generatorRun = false;
  lastGeneratorRun = false;
  generatorStartMillis = 0;
  noInternetSince = 0;
  gTotalRuntimeMs = loadEepromTotalRuntimeMs();

  sensor220.setSensitivity(ZMPT_SENSITIVITY);
}

float getOutputVrms()
{
  return safeGetRmsVoltage(ZMPT_RMS_LOOPS);
}

static inline bool isGeneratorRun_sensor220()
{
  const float vrms = getOutputVrms();
  return (vrms > ZMPT_RMS_THRESHOLD_V);
}

bool pollGeneratorRun(bool force)
{
  const unsigned long now = millis();

  if (!force && !elapsed(lastRunCheck, STATE_POLL_PERIOD_MS))
  {
    return generatorRun;
  }
  lastRunCheck = now;

  // Прямо беремо стан із сенсора
  const bool sensedRun = isGeneratorRun_sensor220();

  generatorRun = sensedRun;
  return generatorRun;
}

void updateGeneratorState(bool force)
{
  const unsigned long now = millis();
  const bool currentRun = pollGeneratorRun(force);

  // 1) Runtime в RAM (оновлюємо часто, але без EEPROM)
  static unsigned long lastTickMs = 0;

  if (currentRun)
  {
    if (lastTickMs == 0)
      lastTickMs = now;

    const unsigned long dt = now - lastTickMs;
    gTotalRuntimeMs += (uint64_t)dt;
    lastTickMs = now;
  }
  else
  {
    lastTickMs = 0;
  }

  // 2) Фронт вимкнення: 1 -> 0
  //    тут і лог, і єдине місце запису в EEPROM (за вашою вимогою)
  if (lastGeneratorRun && !currentRun)
  {
    saveEepromTotalRuntimeMs(gTotalRuntimeMs);
  }

  // 3) Автостоп за тривалої втрати інтернету під час роботи генератора
  if (currentRun)
  {
    if (WiFi.status() != WL_CONNECTED)
    {
      if (noInternetSince == 0)
        noInternetSince = now;
      else if (elapsed(noInternetSince, NET_LOSS_AUTO_STOP_MS))
      {
        (void)stopGenerator();
      }
    }
    else
    {
      noInternetSince = 0;
    }
  }
  else
  {
    noInternetSince = 0;
  }

  // 4) Оновлюємо останній стан
  lastGeneratorRun = currentRun;
}
