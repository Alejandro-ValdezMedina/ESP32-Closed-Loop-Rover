#pragma once
#include <Arduino.h>
#include "Pins.h"

struct TickSnapshot {
  uint32_t dtTicksL;
  uint32_t dtTicksR;
  uint32_t totalTicksL;
  uint32_t totalTicksR;
};

class Encoders {
public:
  void begin();
  TickSnapshot sampleAndReset();

private:
  static void IRAM_ATTR isrL();
  static void IRAM_ATTR isrR();
  static volatile uint32_t _ticksL;
  static volatile uint32_t _ticksR;

  uint32_t _totalL = 0;
  uint32_t _totalR = 0;
};
