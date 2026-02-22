#include <Arduino.h>
#include "Pins.h"
#include "Config.h"
#include "MotorDriver.h"
#include "Encoders.h"
#include "Ultrasonic.h"
#include "ImuBno055.h"
#include "LoggerSD.h"
#include "StateMachine.h"

MotorDriver motors;
Encoders enc;
Ultrasonic ranger;
ImuBno055 imu;
LoggerSD logger;
StateMachine sm;

static uint32_t lastControlMs = 0;

void setup() {
  Serial.begin(115200);
  uint32_t t0 = millis();
  while (!Serial && (millis() - t0) < 3000) delay(10);
  Serial.println("\nBOOT");

  logger.begin();
  motors.begin();
  motors.coast();
  enc.begin();
  ranger.begin();
  imu.begin();

  sm.begin();
  lastControlMs = millis();

  Serial.println("RUN: obstacle avoid + speed PI + SD logging");
}

void loop() {
  const uint32_t now = millis();

  ranger.update(now);
  sm.update(now, ranger);

  if (now - lastControlMs < DT_MS) return;
  lastControlMs = now;

  const TickSnapshot ts = enc.sampleAndReset();
  static float rpmL = 0.0f, rpmR = 0.0f;
  const float rpmL_raw = ticksToRpm(ts.dtTicksL, DT_S);
  const float rpmR_raw = ticksToRpm(ts.dtTicksR, DT_S);
  rpmL = ALPHA_RPM * rpmL + (1.0f - ALPHA_RPM) * rpmL_raw;
  rpmR = ALPHA_RPM * rpmR + (1.0f - ALPHA_RPM) * rpmR_raw;
  const ImuSample imuS = imu.sample();
  float targL = 0.0f, targR = 0.0f;
  int8_t dirL = 0, dirR = 0;

  sm.getControlTargets(targL, targR, dirL, dirR);

  const MotorCmd cmd = motors.speedPI(targL, targR, rpmL, rpmR, ts.dtTicksL, ts.dtTicksR);
  motors.apply(dirL, dirR, cmd.pwmL, cmd.pwmR);

  logger.logRow(now, sm.state(),
                ranger.filteredCm(), ranger.valid(), ranger.lastEchoUs(),
                ts.dtTicksL, ts.dtTicksR, ts.totalTicksL, ts.totalTicksR,
                rpmL, rpmR, targL, targR,
                cmd.pwmL, cmd.pwmR,
                imuS.headingDeg, imuS.imuOK, imuS.cal_sys, imuS.cal_g, imuS.cal_a, imuS.cal_m);


  Serial.print("t="); Serial.print(now);
  Serial.print(" state="); Serial.print(sm.stateName());
  Serial.print(" dist="); Serial.print(ranger.filteredCm(), 1);
  Serial.print(" valid="); Serial.print(ranger.valid() ? 1 : 0);
  Serial.print(" | rpmL="); Serial.print(rpmL, 1); Serial.print(" pwmL="); Serial.print(cmd.pwmL);
  Serial.print(" | rpmR="); Serial.print(rpmR, 1); Serial.print(" pwmR="); Serial.print(cmd.pwmR);
  Serial.print(" | hdg="); Serial.print(imuS.headingDeg, 1);
  Serial.print(" cal="); Serial.print((int)imuS.cal_sys); Serial.print((int)imuS.cal_g);
  Serial.print((int)imuS.cal_a); Serial.print((int)imuS.cal_m);
  Serial.println();
}
