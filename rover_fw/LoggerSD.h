#pragma once
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include "Pins.h"

class LoggerSD {
public:
  void begin();
  void logRow(uint32_t t_ms, uint8_t state,
              float dist_cm, bool dist_valid, uint32_t echo_us,
              uint32_t ticksL_dt, uint32_t ticksR_dt,
              uint32_t ticksL_tot, uint32_t ticksR_tot,
              float rpmL, float rpmR, float targetRPM_L, float targetRPM_R,
              int pwmL, int pwmR,
              float heading_deg,
              uint8_t imu_ok, uint8_t cal_sys, uint8_t cal_g, uint8_t cal_a, uint8_t cal_m);

private:
  bool _ok = false;
  File _f;

  void openLogFile();
};
