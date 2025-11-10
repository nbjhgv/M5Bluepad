//
// Copyright (c) Dmitry Akulov. All rights reserved.
//
// Repository info:     https://github.com/pink0D/GeekShield
// Contact information: pink0D.github@gmail.com
//
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

//
// Instructions for using M5Bluepad
// https://....
//

#include <M5Unified.h>
#include <BluepadHub.h>
#include <NeoPixelStatusIndicator.h>
#include <M5AtomicMotionExt.h>

NeoPixelStatusIndicator LedRGB;     // RGB Led in Atom Lite
M5AtomicMotionExt AtomicMotionExt;  // M5AtomicMotion extension

class : public bluepadhub::ControlProfile {

  uint8_t FWD_motor = CHANNEL_SERVO_1;
  uint8_t RWD_motor = CHANNEL_SERVO_2;
  uint8_t FWD_steer = CHANNEL_SERVO_3;
  uint8_t RWD_steer = CHANNEL_SERVO_4;

  double add_steer_start = 0.75;  // rear steering start threshold

  void begin() {

    AtomicMotionExt.setServoPulseRange(FWD_motor, 500, 2500);
    AtomicMotionExt.setServoPulseRange(RWD_motor, 500, 2500);

    AtomicMotionExt.setServoMaxAngle(FWD_steer, 60);
    AtomicMotionExt.setServoMaxAngle(RWD_steer, 10);
  };

  void update(ControllerPtr ctl) {

    // default controls
    double accelerator = normalizeTriggerInput(ctl->throttle());  // R trigger to accelerate
    double steer = normalizeStickInput(ctl->axisX());             // L stick to steer

    if (ctl->y() || ctl->r1()) // reverse while holding Y or R1
      accelerator = -accelerator;

    // toggle easy mode 
    static bool easyMode = false;
    if (wasClicked(ctl->l1()))
      easyMode = !easyMode;

    if (easyMode) {
      steer = normalizeStickInput(ctl->axisRX());       // R stick to steer
      accelerator = -normalizeStickInput(ctl->axisY()); // FORWARD when L stick is UP (Y < 0)
    }
    
    double steer1 = 0;
    double steer2 = 0;

    // front axle steering is set directly from input
    steer1 = steer; 

    // rear steering is added only when input is greater than threshold 
    if (steer > add_steer_start)
      steer2 = (steer - add_steer_start) / (1.0 - add_steer_start);
    if (steer < -add_steer_start) 
      steer2 = (steer + add_steer_start) / (1.0 - add_steer_start);

    AtomicMotionExt.updateServo(FWD_motor, accelerator);
    AtomicMotionExt.updateServo(RWD_motor, accelerator);
    AtomicMotionExt.updateServo(FWD_steer, -steer1); // front and rear steering have opposite directions
    AtomicMotionExt.updateServo(RWD_steer, steer2);
  };

  void failsafe() {
    AtomicMotionExt.stopMotors();
  };

} MonsterProfile;

void setup() {
  
  Serial.begin(115200);

  M5.begin();
  
  LedRGB.setBrightness(20);
  LedRGB.begin();

  BluepadHub.setStatusIndicator(&LedRGB);
  BluepadHub.setControlProfile(&MonsterProfile);
  BluepadHub.begin(); 

  while(!AtomicMotionExt.begin()) {
     Serial.println("Atomic Motion begin failed");
     LedRGB.setStatusPattern(bluepadhub::StatusIndicator::StatusPattern::Error);
     delay(1000);
  }

  Serial.println("Setup finished");
}

void loop() {
  // updates
  M5.update();
  BluepadHub.update();

  // hold button for 5 secs to enter pairing mode
  static bool hold1 = false;
  if (M5.BtnA.pressedFor(5000)) {
    if (!hold1) {
      BluepadHub.enablePairing();
      hold1 = true;
    }
  }

  // hold button for 10 secs to forget paired gamepads and enter pairing mode
  static bool hold2 = false;
  if (M5.BtnA.pressedFor(10000)) {
    if (!hold2) {
      BluepadHub.resetPairing();
      BluepadHub.enablePairing();
      hold2 = true;
    }
  }

  // reset hold state manually since pressedFor(ms) in M5Unified returns 'true' multiple times 
  if (M5.BtnA.wasReleased()) {
    hold1 = false;
    hold2 = false;
  } 

  //no delay here because it's inside BluepadHub.update();
}
