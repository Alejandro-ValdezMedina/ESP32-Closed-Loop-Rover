#pragma once
#include <Arduino.h>
#include "Pins.h"
#include "Config.h"

class Ultrasonic {
public:
  void begin();
  void update(uint32_t now);

  bool valid() const { return _valid; }
  float filteredCm() const { return _distFilt; }
  uint32_t lastEchoUs() const { return _lastEchoUs; }

private:
  float _distFilt = NAN;
  bool _valid = false;
  uint32_t _lastValidMs = 0;
  uint32_t _lastPingMs = 0;
  uint32_t _lastEchoUs = 0;

  float readCm(uint32_t* echoUsOut);
};
