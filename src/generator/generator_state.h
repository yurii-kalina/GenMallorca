// GeneratorState.h
#pragma once
#include <stdint.h>

extern bool generatorRun;
extern unsigned long generatorStartMillis;
extern uint64_t gTotalRuntimeMs;

void initGeneratorState();
void updateGeneratorState(bool force = false);
bool pollGeneratorRun(bool force = false);
float getOutputVrms();