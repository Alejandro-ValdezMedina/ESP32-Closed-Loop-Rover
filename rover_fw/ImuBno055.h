#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

struct ImuSample {
  float headingDeg;
  uint8_t imuOK;
  uint8_t cal_sys, cal_g, cal_a, cal_m;
};

class ImuBno055 {
public:
  bool begin();
  ImuSample sample() const;

private:
  mutable Adafruit_BNO055 _bno = Adafruit_BNO055(55);
  bool _ok = false;
};
