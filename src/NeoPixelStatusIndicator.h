//
// Copyright (c) Dmitry Akulov. All rights reserved.
//
// Repository info:     https://github.com/pink0D/GeekShield
// Contact information: pink0D.github@gmail.com
//
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#ifndef _NEOPIXEL_STATUS_INDICATOR_H_
#define _NEOPIXEL_STATUS_INDICATOR_H_

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "BluepadHub.h"

class NeoPixelStatusIndicator : public bluepadhub::StatusIndicator {
  public:

    NeoPixelStatusIndicator() {};

    virtual void begin(int16_t pin = 27);

    virtual void showStatusPattern();
    virtual void showEventPattern();

    void setBrightness(uint8_t _ledBrightness);

  private:
    Adafruit_NeoPixel *pixels = nullptr;
    void setLedColor(unsigned long color);

    uint8_t ledBrightness = 255;
};  

#endif