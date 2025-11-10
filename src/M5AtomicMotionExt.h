//
// Copyright (c) Dmitry Akulov. All rights reserved.
//
// Repository info:     https://github.com/pink0D/M5Bluepad
// Contact information: pink0D.github@gmail.com
//
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#ifndef _M5_ATOMIC_MOTION_EXT_H_
#define _M5_ATOMIC_MOTION_EXT_H_

#include <M5AtomicMotion.h>

#define M5_ATOMIC_MOTION_SERVO_COUNT 4
#define M5_ATOMIC_MOTION_MOTOR_COUNT 2

#define CHANNEL_SERVO_1 0
#define CHANNEL_SERVO_2 1
#define CHANNEL_SERVO_3 2
#define CHANNEL_SERVO_4 3

#define CHANNEL_MOTOR_1 0
#define CHANNEL_MOTOR_2 1

class M5AtomicMotionExt : public M5AtomicMotion {
  public:
    M5AtomicMotionExt();
  
    void stopMotors();

    void setAntiJitter(double _antiJitter) { 
      antiJitter = _antiJitter;
    };

    void setServoPulseRange(uint8_t servo_ch, uint16_t pulse_min, uint16_t pulse_max);
    void setServoMaxAngle(uint8_t servo_ch, uint16_t angle_max);
    void setMotorLimits(uint8_t motor_ch, double limit_min, double limit_max);

    void updateServo(uint8_t servo_ch, double normalized_value);
    void updateMotor(uint8_t motor_ch, double normalized_value);

  private:

    double antiJitter = 0.025;

    class ChannelFilter {

      public:
        ChannelFilter() {};    

        void setLimits(double _limitMin, double _limitMax);
        void setAntiJitter(double _antiJitter);

        // returns new filtered value
        double updateValue(double newValue);

      private:
        double antiJitter;
        double limitMin = 0.0;
        double limitMax = 1.0;
        double value = 0.0;
    };

    uint16_t servoPulseMin[M5_ATOMIC_MOTION_SERVO_COUNT];
    uint16_t servoPulseMax[M5_ATOMIC_MOTION_SERVO_COUNT];

    ChannelFilter servoFilter[M5_ATOMIC_MOTION_SERVO_COUNT];
    ChannelFilter motorFilter[M5_ATOMIC_MOTION_MOTOR_COUNT];
};

#endif