# ESP32-Closed-Loop-Rover

A real-time embedded control system built on an ESP32-S2 Feather driving a differential-drive mobile robot.

This project demonstrates closed-loop motor control, encoder-based velocity estimation, gyro-based heading stabilization, obstacle detection, and structured state-machine architecture — implemented in embedded C++.

## System Overview

This rover is a two-wheel differential drive platform that:

- Maintains controlled wheel velocity using encoder feedback
- Stabilizes heading using IMU gyro integration
- Detects obstacles with an ultrasonic sensor
- Runs structured state-based autonomous logic
- Logs telemetry for validation and analysis

## Hardware Architecture

### Microcontroller
- **ESP32-S2 Feather**
- 240 MHz
- 3.3V logic
- Interrupt-driven encoder handling
- Real-time control loop (~200 ms update rate)

### Motor Driver
- **TB6612FNG Dual H-Bridge**
- Independent PWM control per motor
- Separate motor supply rail (5–6V)

### Motors
- 2x DC gear motors (~140 RPM rated)
- Differential drive configuration
- Rear caster for stability

### Encoders
- 20-slot wheel encoder disks
- 1 pulse per slot
- Interrupt-driven tick counting
- RPM calculated from tick frequency

### IMU
- **BNO055**
- Gyro-based yaw estimation
- Heading stabilization via yaw integration
- Bias auto-calibration on startup

### Ultrasonic Sensor
- HC-SR04
- Front-mounted
- Obstacle distance measurement
- 5V → 3.3V level shifted echo signal

### Power Architecture

Two-rail system:

**Logic Rail (3.3V):**
- ESP32
- IMU
- Encoder VCC
- TB6612 logic

**Motor / Sensor Rail (~5–6V from 4xAA):**
- Motor driver VM
- Ultrasonic sensor
- Servo (future expansion)

All grounds common.
470µF bulk capacitor across VM for motor noise suppression.

## Control Architecture

### 1. Velocity Control (Per Wheel)

Each wheel runs a closed-loop PI controller:

- Encoder ISR counts ticks
- RPM computed every control interval
- Low-pass filtered velocity
- Feedforward + PI correction
- PWM output clamped to safe range

This ensures consistent speed despite:
- Battery voltage sag
- Surface friction variation
- Motor asymmetry

### 2. Gyro-Based Heading Hold

Yaw rate is obtained from `gyro.z()`.

Startup sequence:
- 2-second stationary bias estimation
- Gyro bias stored
- Relative yaw integrated over time

Heading error:
- yaw_error = yaw_target - yaw_est
- deltaRPM = Kp * yaw_error


Wheel targets adjusted:
- left_target = baseRPM - deltaRPM
- right_target = baseRPM + deltaRPM


This allows:
- Straight-line stabilization
- Controlled drift correction
- Stable heading without magnetometer dependency

### 3. Obstacle Detection (Planned Integration)

State machine behavior:

DRIVE_FORWARD
↓ (distance < threshold)
STOP
↓
TURN
↓
DRIVE_FORWARD

Ultrasonic sensor mounted at front chassis slit.

Echo signal level shifted via resistor divider:
- 100k (top)
- 68k (bottom)

### 4. State Machine

Structured states:
- IDLE
- DRIVE_FORWARD
- TURN
- OBSTACLE_AVOID
- STOP

No blocking delays.
All logic driven by timed control loop.

## Real-Time Design Considerations
- Interrupt-driven encoder counting
- Deterministic control interval (DT = 200 ms)
- Separate motor and logic rails
- Bulk decoupling on motor supply
- Gyro bias estimation at boot
- Output saturation protection
- Integral windup clamping

## Engineering Challenges Solved
- Motor voltage sag compensation
- Encoder quantization smoothing
- Gyro bias drift removal
- Heading oscillation mitigation
- Ground noise isolation
- Safe level shifting from 5–6V to 3.3V logic
- Differential drive tuning

## Measured Performance
- Stable ~60 RPM per wheel under load
- ~0.2 m/s linear velocity
- Heading correction within a few degrees over short runs
- No brownouts under dual motor load

## Future Improvements
- SD card telemetry logging
- Obstacle avoidance with servo scanning
- Sensor fusion refinement
- Wheel odometry position tracking
- PID tuning optimization
- FreeRTOS task separation

## Keywords
Embedded C++ development, Real-time control systems, Hardware/software co-design, Closed-loop motor control, Sensor integration, System-level debugging, Robotics, Autonomous systems, Mechatronics, Firmware development, Control systems engineering

Built as a ground-up embedded systems platform using an ESP32 microcontroller and discrete moto
