#pragma once
#include <Arduino.h>

static constexpr float TICKS_PER_REV = 20.0f;
static constexpr uint32_t DT_MS = 200;
static constexpr float DT_S = DT_MS / 1000.0f;
static constexpr float DRIVE_RPM = 30.0f;
static constexpr float TURN_RPM  = 25.0f;
static constexpr int PWM_MIN = 35;
static constexpr int PWM_MAX = 160;
static constexpr float ALPHA_RPM = 0.7f;
static constexpr float OBSTACLE_CM = 25.0f;
static constexpr float CLEAR_CM    = 35.0f;
static constexpr uint32_t MIN_TURN_MS  = 500;
static constexpr uint32_t MIN_DRIVE_MS = 400;
static constexpr float ALPHA_DIST = 0.6f;
static constexpr uint32_t DIST_TIMEOUT_MS = 500;

inline float ticksToRpm(uint32_t ticks, float dt_s) {
  return ((ticks / dt_s) / TICKS_PER_REV) * 60.0f;
}
