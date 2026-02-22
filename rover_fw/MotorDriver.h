#pragma once
#include <Arduino.h>
#include "Pins.h"
#include "Config.h"

struct MotorCmd {
  int pwmL;
  int pwmR;
};

class MotorDriver {
public:
  void begin();
  void coast();

  MotorCmd speedPI(float targL, float targR, float rpmL, float rpmR,
                   uint32_t dtTicksL, uint32_t dtTicksR);

  void apply(int8_t dirL, int8_t dirR, int pwmL, int pwmR);

private:
  int _basePWM_L = 98;
  int _basePWM_R = 86;
  float _Kp = 0.8f;
  float _Ki = 1.2f;

  float _iL = 0.0f;
  float _iR = 0.0f;

  int clampInt(int x, int lo, int hi);
  float clampF(float x, float lo, float hi);

  void setMotorA(int8_t dir, int pwm);
  void setMotorB(int8_t dir, int pwm);
};
