#include "StateMachine.h"

void StateMachine::update(uint32_t now, const Ultrasonic& u) {
  const bool distValid = u.valid();
  const float d = u.filteredCm();
  const bool obstacle = distValid && (d < OBSTACLE_CM);
  const bool clear    = distValid && (d > CLEAR_CM);

  if (!distValid) {
    if (_state != STOP_NO_ECHO) { _state = STOP_NO_ECHO; _enterMs = now; }
    return;
  }

  if (_state == STOP_NO_ECHO) { _state = DRIVE; _enterMs = now; }

  if (_state == DRIVE) {
    if ((now - _enterMs) >= MIN_DRIVE_MS && obstacle) {
      _state = TURN_R; _enterMs = now;
    }
  } else if (_state == TURN_R) {
    if ((now - _enterMs) >= MIN_TURN_MS && clear) {
      _state = DRIVE; _enterMs = now;
    }
  }
}

void StateMachine::getControlTargets(float& targL, float& targR, int8_t& dirL, int8_t& dirR) const {
  if (_state == DRIVE) {
    targL = DRIVE_RPM; targR = DRIVE_RPM;
    dirL = +1; dirR = +1;
  } else if (_state == TURN_R) {
    targL = TURN_RPM; targR = TURN_RPM;
    dirL = +1; dirR = -1;
  } else {
    targL = 0.0f; targR = 0.0f;
    dirL = 0; dirR = 0;
  }
}

const char* StateMachine::stateName() const {
  switch (_state) {
    case DRIVE: return "DRIVE";
    case TURN_R: return "TURN_R";
    case STOP_NO_ECHO: return "STOP_NO_ECHO";
    default: return "UNK";
  }
}
