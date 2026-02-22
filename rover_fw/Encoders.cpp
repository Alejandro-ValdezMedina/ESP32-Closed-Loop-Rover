#include "Encoders.h"

volatile uint32_t Encoders::_ticksL = 0;
volatile uint32_t Encoders::_ticksR = 0;

void IRAM_ATTR Encoders::isrL() { _ticksL++; }
void IRAM_ATTR Encoders::isrR() { _ticksR++; }

void Encoders::begin() {
  pinMode(PIN_ENC_L, INPUT_PULLUP);
  pinMode(PIN_ENC_R, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_L), Encoders::isrL, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_R), Encoders::isrR, FALLING);
}

TickSnapshot Encoders::sampleAndReset() {
  noInterrupts();
  uint32_t l = _ticksL; _ticksL = 0;
  uint32_t r = _ticksR; _ticksR = 0;
  interrupts();

  _totalL += l;
  _totalR += r;

  return { l, r, _totalL, _totalR };
}
