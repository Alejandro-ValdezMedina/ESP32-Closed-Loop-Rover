#include "LoggerSD.h"

void LoggerSD::begin() {
  Serial.println("SD init...");
  SPI.begin();
  pinMode(PIN_SD_CS, OUTPUT);
  digitalWrite(PIN_SD_CS, HIGH);

  _ok = SD.begin(PIN_SD_CS, SPI, 1000000);
  if (!_ok) {
    Serial.println("SD.begin FAILED (logging disabled).");
    return;
  }
  Serial.println("SD.begin OK");
  openLogFile();
}

void LoggerSD::openLogFile() {
  char name[20];
  for (int i = 0; i < 100; i++) {
    snprintf(name, sizeof(name), "/LOG%02d.CSV", i);
    if (!SD.exists(name)) {
      _f = SD.open(name, FILE_WRITE);
      if (_f) {
        _f.println("t_ms,state,dist_cm,dist_valid,echo_us,"
                   "ticksL,ticksR,ticksL_total,ticksR_total,"
                   "rpmL,rpmR,targetRPM_L,targetRPM_R,pwmL,pwmR,"
                   "heading_deg,imu_ok,cal_sys,cal_g,cal_a,cal_m");
        _f.flush();
        Serial.print("Logging to "); Serial.println(name);
      } else {
        Serial.println("ERROR: Failed to open log file.");
      }
      return;
    }
  }
  Serial.println("ERROR: No free /LOGxx.CSV filename.");
}

void LoggerSD::logRow(uint32_t t_ms, uint8_t state,
                      float dist_cm, bool dist_valid, uint32_t echo_us,
                      uint32_t ticksL_dt, uint32_t ticksR_dt,
                      uint32_t ticksL_tot, uint32_t ticksR_tot,
                      float rpmL, float rpmR, float targetRPM_L, float targetRPM_R,
                      int pwmL, int pwmR,
                      float heading_deg,
                      uint8_t imu_ok, uint8_t cal_sys, uint8_t cal_g, uint8_t cal_a, uint8_t cal_m) {
  if (!_ok || !_f) return;

  _f.print(t_ms); _f.print(',');
  _f.print(state); _f.print(',');

  if (isnan(dist_cm)) _f.print("nan"); else _f.print(dist_cm, 1);
  _f.print(',');

  _f.print(dist_valid ? 1 : 0); _f.print(',');
  _f.print(echo_us); _f.print(',');

  _f.print(ticksL_dt); _f.print(',');
  _f.print(ticksR_dt); _f.print(',');
  _f.print(ticksL_tot); _f.print(',');
  _f.print(ticksR_tot); _f.print(',');

  _f.print(rpmL, 1); _f.print(',');
  _f.print(rpmR, 1); _f.print(',');
  _f.print(targetRPM_L, 1); _f.print(',');
  _f.print(targetRPM_R, 1); _f.print(',');

  _f.print(pwmL); _f.print(',');
  _f.print(pwmR); _f.print(',');

  if (isnan(heading_deg)) _f.print("nan"); else _f.print(heading_deg, 1);
  _f.print(',');

  _f.print((int)imu_ok); _f.print(',');
  _f.print((int)cal_sys); _f.print(',');
  _f.print((int)cal_g); _f.print(',');
  _f.print((int)cal_a); _f.print(',');
  _f.print((int)cal_m);
  _f.println();

  static uint8_t flushDiv = 0;
  if (++flushDiv >= 5) { flushDiv = 0; _f.flush(); }
}
