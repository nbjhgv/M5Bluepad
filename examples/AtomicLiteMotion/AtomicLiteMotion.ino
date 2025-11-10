//
// Copyright (c) Dmitry Akulov. All rights reserved.
//
// Repository info:     https://github.com/pink0D/GeekShield
// Contact information: pink0D.github@gmail.com
//
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include <M5Unified.h>
#include <BluepadHub.h>
#include <NeoPixelStatusIndicator.h>
#include <M5AtomicMotionExt.h>

NeoPixelStatusIndicator LedRGB;     // RGB Led in Atom Lite
M5AtomicMotionExt AtomicMotionExt;  // M5AtomicMotion extension


// this class handles controller input ans sets channel outputs

class : public bluepadhub::ControlProfile {

  // set channel parameters here
  void begin() {

    // if no pulse range is specified, then default 1000-2000us will be used
    AtomicMotionExt.setServoPulseRange(CHANNEL_SERVO_1, 500, 2500);
    AtomicMotionExt.setServoPulseRange(CHANNEL_SERVO_2, 500, 2500);
    AtomicMotionExt.setServoPulseRange(CHANNEL_SERVO_3, 500, 2500);
    AtomicMotionExt.setServoPulseRange(CHANNEL_SERVO_4, 500, 2500);

    // alternatevely, you can specify servo rotation angle in degrees
    // this will be recalculated to corresponding pulse range internally
    // AtomicMotionExt.setServoMaxAngle(CHANNEL_SERVO_1, 45);
    // AtomicMotionExt.setServoMaxAngle(CHANNEL_SERVO_2, 90);

    // PWM duty cycle for M1 & M2 can be limited from 0 to 100% (default is full range)
    // usually this is not needed, but some motors start only when PWM is more than 25-30%
    // AtomicMotionExt.setMotorLimits(CHANNEL_MOTOR_1, 0.25, 1.0);

    // if necessary, the upper PWM limit can be also specified to limit maximum motor power 
    // AtomicMotionExt.setMotorLimits(CHANNEL_MOTOR_2, 0.25, 0.75);
  };

  // process updates from controller
  void update(ControllerPtr ctl) {

    static int reverse1 = 1;
    static int reverse2 = 1;

    if (wasClicked(ctl->l1()))
      reverse1 = -reverse1;

    if (wasClicked(ctl->r1()))
      reverse2 = -reverse2;


    // updateServo/updateMotor methods set new values for servo/motor channels output with anti-jitter filtering
    // argument is expected to be a normalized value between -1.0 and +1.0
    // 1.0 = full speed forward / max servo rotation clockwise
    // -1.0 = full speed reverse / max servo rotation counter-clockwise
    // 0.0 stops motors / returns servo to zero position
    // 0.5 / 0.75 or any other double value = set motor speed / servo rotation to 50% / 75% / etc

    // normalizeStickInput/normalizeTriggerInput methods apply deadzone correction to raw values
    // then integer values from controller are mapped to normalized range (-1.0, 1.0) 
   
    AtomicMotionExt.updateServo(CHANNEL_SERVO_1, normalizeStickInput(ctl->axisX()));
    AtomicMotionExt.updateServo(CHANNEL_SERVO_2, normalizeStickInput(ctl->axisY()));
    AtomicMotionExt.updateServo(CHANNEL_SERVO_3, normalizeStickInput(ctl->axisRX()));
    AtomicMotionExt.updateServo(CHANNEL_SERVO_4, normalizeStickInput(ctl->axisRY()));

    AtomicMotionExt.updateMotor(CHANNEL_MOTOR_1, normalizeTriggerInput(ctl->brake())*reverse1);
    AtomicMotionExt.updateMotor(CHANNEL_MOTOR_2, normalizeTriggerInput(ctl->throttle())*reverse2);

    // for more details on controller data processing see TestBluePad32 example and Bluepad32 docs
  };

  // this method is called when no data is received from controller after timeout
  void failsafe() {
    AtomicMotionExt.stopMotors();
  };

} TestProfile;

//  Arduino setup function
void setup() {
  
  Serial.begin(115200);

  M5.begin();
  
  LedRGB.setBrightness(20);
  LedRGB.begin();

  BluepadHub.setStatusIndicator(&LedRGB);
  BluepadHub.setControlProfile(&TestProfile);
  BluepadHub.begin();

  // begin() can be called with config params to adjust controller settings
  // actual deadzone values depend on type of controller used
  // BluePad32 example can be used to analyze the raw values sent by controller

  /*
  auto cfg = BluepadHubConfig();
  cfg.controllerStickDeadzoneLow = 50;        // 0 = lowest value for stick input
  cfg.controllerStickDeadzoneHigh = 500;      // 512 = highest value for stick input
  cfg.controllerTriggerDeadzoneLow = 5;       // 0 = lowest value for trigger input
  cfg.controllerTriggerDeadzoneHigh = 1000;   // 1024 = highest value for trigger input 

  BluepadHub.begin(cfg);
  */

  while(!AtomicMotionExt.begin()) {
     Serial.println("Atomic Motion begin failed");
     LedRGB.setStatusPattern(bluepadhub::StatusIndicator::StatusPattern::Error);
     delay(1000);
  }

  Serial.println("Setup finished");
}

// Arduino loop function
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

  //no delay here because it's inside BluepadHub.update()
}
