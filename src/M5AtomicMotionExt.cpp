//
// Copyright (c) Dmitry Akulov. All rights reserved.
//
// Repository info:     https://github.com/pink0D/M5Bluepad
// Contact information: pink0D.github@gmail.com
//
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "M5AtomicMotionExt.h"

void M5AtomicMotionExt::ChannelFilter::setLimits(double _limitMin, double _limitMax) {
  if (_limitMin < 0)
    _limitMin = 0;

  if (_limitMax > 1.0)
    _limitMax = 1.0;

  limitMin = _limitMin;
  limitMax = _limitMax;
}

void M5AtomicMotionExt::ChannelFilter::setAntiJitter(double _antiJitter) { 
  antiJitter = _antiJitter;
}

double M5AtomicMotionExt::ChannelFilter::updateValue(double newValue) {

  if (newValue < -1.0)
    newValue = -1.0;

  if (newValue > 1.0)
    newValue = 1.0;

  double calc_value = 0.0; // calculate new value
  
  if (newValue > antiJitter)
    calc_value = limitMin + (limitMax - limitMin) * newValue;

  if (newValue < -antiJitter)
    calc_value = -limitMin + (limitMax - limitMin) * newValue;

  if (calc_value < -limitMax+antiJitter)
    calc_value = -limitMax;

  if (calc_value > limitMax-antiJitter)
    calc_value = limitMax;

  if (abs(value-calc_value) > antiJitter)
    value = calc_value;

  if (abs(calc_value) < antiJitter)
    value = 0;

  return value;
}

M5AtomicMotionExt::M5AtomicMotionExt() {
  for (int servo_ch = 0; servo_ch < M5_ATOMIC_MOTION_SERVO_COUNT; servo_ch++) {
    servoPulseMin[servo_ch] = 1000;
    servoPulseMax[servo_ch] = 2000;

    servoFilter[servo_ch].setAntiJitter(antiJitter);
  }

  for (int motor_ch = 0; motor_ch < M5_ATOMIC_MOTION_MOTOR_COUNT; motor_ch++) {
    motorFilter[motor_ch].setAntiJitter(antiJitter);
  }
}

void M5AtomicMotionExt::stopMotors() {

  for (int servo_ch = 0; servo_ch < M5_ATOMIC_MOTION_SERVO_COUNT; servo_ch++)
    setServoPulse(servo_ch, 1500);

  for (int motor_ch = 0; motor_ch < M5_ATOMIC_MOTION_MOTOR_COUNT; motor_ch++)
    setMotorSpeed(motor_ch, 0);
}

void M5AtomicMotionExt::setMotorLimits(uint8_t motor_ch, double limit_min, double limit_max) {

  if (motor_ch >= M5_ATOMIC_MOTION_MOTOR_COUNT)
    return;

  motorFilter[motor_ch].setLimits(limit_min, limit_max);
}

void M5AtomicMotionExt::setServoPulseRange(uint8_t servo_ch, uint16_t pulse_min, uint16_t pulse_max) {

  if (servo_ch >= M5_ATOMIC_MOTION_SERVO_COUNT)
    return;

  if (pulse_min < 500)
    pulse_min = 500;
  if (pulse_max > 2500)
    pulse_max = 2500;

  servoPulseMin[servo_ch] = pulse_min;
  servoPulseMax[servo_ch] = pulse_max;
}

void M5AtomicMotionExt::setServoMaxAngle(uint8_t servo_ch, uint16_t angle_max) {

  if (servo_ch >= M5_ATOMIC_MOTION_SERVO_COUNT)
    return;

  if (angle_max > 180)
    angle_max = 180;

  double delta = 500.0 * ( ((double) angle_max) / 90.0 );

  servoPulseMin[servo_ch] = 1500 - delta;
  servoPulseMax[servo_ch] = 1500 + delta;
}

void M5AtomicMotionExt::updateMotor(uint8_t motor_ch, double normalized_value) {

  if (motor_ch >= M5_ATOMIC_MOTION_MOTOR_COUNT)
    return;

  double speed = motorFilter[motor_ch].updateValue(normalized_value) * 127.0;

  setMotorSpeed(motor_ch, (int8_t) speed);
}

void M5AtomicMotionExt::updateServo(uint8_t servo_ch, double normalized_value) {

  if (servo_ch >= M5_ATOMIC_MOTION_SERVO_COUNT)
    return;

  double mid   = (servoPulseMax[servo_ch] + servoPulseMin[servo_ch]) / 2.0;
  double width = (servoPulseMax[servo_ch] - servoPulseMin[servo_ch]) / 2.0;

  double pulse = mid + width * servoFilter[servo_ch].updateValue(normalized_value);

  setServoPulse(servo_ch, (uint16_t) pulse);
}
