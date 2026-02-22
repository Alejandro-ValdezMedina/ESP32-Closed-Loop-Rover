#include "Ultrasonic.h"

void Ultrasonic::begin() {
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
}

float Ultrasonic::readCm(uint32_t* echoUsOut) {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  uint32_t us = pulseIn(PIN_ECHO, HIGH, 25000UL);
  if (echoUsOut) *echoUsOut = us;
  if (us == 0) return NAN;
  return (float)us / 58.0f;
}

void Ultrasonic::update(uint32_t now) {
  if (now - _lastPingMs < 120) {
    _valid = (_lastValidMs != 0) && ((now - _lastValidMs) <= DIST_TIMEOUT_MS);
    return;
  }
  _lastPingMs = now;

  uint32_t echoUs = 0;
  float d = readCm(&echoUs);
  _lastEchoUs = echoUs;

  if (!isnan(d)) {
    _lastValidMs = now;
    if (isnan(_distFilt)) _distFilt = d;
    else _distFilt = ALPHA_DIST * _distFilt + (1.0f - ALPHA_DIST) * d;
  }

  _valid = (_lastValidMs != 0) && ((now - _lastValidMs) <= DIST_TIMEOUT_MS);
}
