#include "MotorDriver.h"

void MotorDriver::begin() {
  pinMode(PIN_PWMA, OUTPUT);
  pinMode(PIN_AIN1, OUTPUT);
  pinMode(PIN_AIN2, OUTPUT);

  pinMode(PIN_PWMB, OUTPUT);
  pinMode(PIN_BIN1, OUTPUT);
  pinMode(PIN_BIN2, OUTPUT);
}

void MotorDriver::coast() {
  setMotorA(0, 0);
  setMotorB(0, 0);
}

MotorCmd MotorDriver::speedPI(float targL, float targR, float rpmL, float rpmR,
                              uint32_t dtTicksL, uint32_t dtTicksR) {
  const float eL = targL - rpmL;
  const float eR = targR - rpmR;

  _iL += eL * DT_S;
  _iR += eR * DT_S;

  if (targL > 0.1f && dtTicksL == 0) _iL = 0.0f;
  if (targR > 0.1f && dtTicksR == 0) _iR = 0.0f;

  const float I_MAX = 30.0f;
  _iL = clampF(_iL, -I_MAX, I_MAX);
  _iR = clampF(_iR, -I_MAX, I_MAX);

  int pwmL = (int)roundf(_basePWM_L + (_Kp * eL) + (_Ki * _iL));
  int pwmR = (int)roundf(_basePWM_R + (_Kp * eR) + (_Ki * _iR));

  if (targL <= 0.1f) pwmL = 0;
  if (targR <= 0.1f) pwmR = 0;

  if (pwmL > 0) pwmL = clampInt(pwmL, PWM_MIN, PWM_MAX);
  if (pwmR > 0) pwmR = clampInt(pwmR, PWM_MIN, PWM_MAX);

  return { pwmL, pwmR };
}

void MotorDriver::apply(int8_t dirL, int8_t dirR, int pwmL, int pwmR) {
  setMotorA(dirL, pwmL);
  setMotorB(dirR, pwmR);
}

int MotorDriver::clampInt(int x, int lo, int hi) {
  if (x < lo) return lo;
  if (x > hi) return hi;
  return x;
}

float MotorDriver::clampF(float x, float lo, float hi) {
  if (x < lo) return lo;
  if (x > hi) return hi;
  return x;
}

void MotorDriver::setMotorA(int8_t dir, int pwm) {
  if (dir == 0 || pwm <= 0) {
    analogWrite(PIN_PWMA, 0);
    digitalWrite(PIN_AIN1, LOW);
    digitalWrite(PIN_AIN2, LOW);
    return;
  }
  pwm = clampInt(pwm, 0, 255);
  if (dir > 0) { digitalWrite(PIN_AIN1, HIGH); digitalWrite(PIN_AIN2, LOW); }
  else         { digitalWrite(PIN_AIN1, LOW);  digitalWrite(PIN_AIN2, HIGH); }
  analogWrite(PIN_PWMA, pwm);
}

void MotorDriver::setMotorB(int8_t dir, int pwm) {
  if (dir == 0 || pwm <= 0) {
    analogWrite(PIN_PWMB, 0);
    digitalWrite(PIN_BIN1, LOW);
    digitalWrite(PIN_BIN2, LOW);
    return;
  }
  pwm = clampInt(pwm, 0, 255);
  if (dir > 0) { digitalWrite(PIN_BIN1, HIGH); digitalWrite(PIN_BIN2, LOW); }
  else         { digitalWrite(PIN_BIN1, LOW);  digitalWrite(PIN_BIN2, HIGH); }
  analogWrite(PIN_PWMB, pwm);
}
