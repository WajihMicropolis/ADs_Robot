#ifndef _LIGHTS_HPP_
#define _LIGHTS_HPP_

#include <Arduino.h>
#include <FastLED.h>

class LED
{
private:
#define LED_Pin 27
#define NUM_LEDS 144

#define SSR_Left 2
#define SSR_Brake 4
#define SSR_Green 5
#define SSR_Right 18

    CRGBArray<NUM_LEDS> leds;
    CRGB prevCol;

    uint64_t ledPrevMill = 0;
    uint8_t idx = 0, idx_max = 255;
    bool increment = true,
         FUllClear = false;

    const unsigned long SSR_High_Delay = 750;
    const unsigned long SSR_Low_Delay = 100;

    void fade(CRGB col);
    void Clear();
    void steering(float angular_z);

public:
    void update(float linear_x, float angular_z, bool chan3);

    LED();
    ~LED();
};

#endif //_LIGHTS_HPP_