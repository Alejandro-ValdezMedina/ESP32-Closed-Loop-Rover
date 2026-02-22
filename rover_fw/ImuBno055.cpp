#include "ImuBno055.h"

bool ImuBno055::begin() {
  _ok = _bno.begin();
  if (_ok) _bno.setExtCrystalUse(true);
  return _ok;
}

ImuSample ImuBno055::sample() const {
  ImuSample s{};
  s.imuOK = _ok ? 1 : 0;
  s.headingDeg = NAN;

  s.cal_sys = s.cal_g = s.cal_a = s.cal_m = 0;

  if (_ok) {
    s.headingDeg = _bno.getVector(Adafruit_BNO055::VECTOR_EULER).x();
    _bno.getCalibration(&s.cal_sys, &s.cal_g, &s.cal_a, &s.cal_m);
  }
  return s;
}
