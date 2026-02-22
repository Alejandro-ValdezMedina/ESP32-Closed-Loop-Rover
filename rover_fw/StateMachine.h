#pragma once
#include <Arduino.h>
#include "Config.h"
#include "Ultrasonic.h"

class StateMachine {
public:
  static constexpr uint8_t DRIVE = 0;
  static constexpr uint8_t TURN_R = 1;
  static constexpr uint8_t STOP_NO_ECHO = 2;

  void begin() { _state = DRIVE; _enterMs = millis(); }

  void update(uint32_t now, const Ultrasonic& u);

  void getControlTargets(float& targL, float& targR, int8_t& dirL, int8_t& dirR) const;

  uint8_t state() const { return _state; }
  const char* stateName() const;

private:
  uint8_t _state = DRIVE;
  uint32_t _enterMs = 0;
};
