#include "Lights.hpp"

LED::LED(/* args */)
{
    FastLED.addLeds<NEOPIXEL, LED_Pin>(leds, NUM_LEDS);

    pinMode(SSR_Left, OUTPUT);
    pinMode(SSR_Brake, OUTPUT);
    pinMode(SSR_Green, OUTPUT);
    pinMode(SSR_Right, OUTPUT);
}

void LED::Clear()
{
    if (FUllClear)
        return;
    // for (int i = 0; i < NUM_LEDS; i++)
    // {

    //     // leds[i] = CHSV(hue, 255, 255);
    // }
    leds.fadeToBlackBy(0); // fade everything out
    FastLED.show();
    FUllClear = true;
    this->idx = 0;
}
void LED::fade(CRGB col)
{
    CRGB color;
    if (col.r != prevCol.r || col.g != prevCol.g || col.b != prevCol.b)
    {
        Serial.println("not equal");
        idx = 0;
    }

    if (millis() - ledPrevMill > 5)
    {
        idx_max = max(col.r, max(col.g, col.b));

        color.r = (col.r != 0) ? idx : 0;
        color.g = (col.g != 0) ? idx : 0;
        color.b = (col.b != 0) ? idx : 0;

        FastLED.showColor(color);
        increment ? idx++ : idx--;

        increment = idx == idx_max ? LOW : idx == 0 ? HIGH
                                                    : increment;

        ledPrevMill = millis();
        FUllClear = false;
    }
    prevCol = col;
}
static unsigned long prevMillRun = 0;

void LED::steering(float angular_z)
{
    if (angular_z != 0)
    {
        if (millis() - prevMillRun <= SSR_High_Delay)
        {
            if (angular_z > 0.0)
            {
                // Serial.println("Left HIGH");
                digitalWrite(SSR_Left, HIGH);
                digitalWrite(SSR_Right, LOW);
            }
            else if (angular_z < 0.0)
            {
                // Serial.println("Right HIGH");
                digitalWrite(SSR_Left, LOW);
                digitalWrite(SSR_Right, HIGH);
            }
        }
        else if (millis() - prevMillRun <= (SSR_High_Delay + SSR_Low_Delay))
        {
            digitalWrite(SSR_Right, LOW);
            digitalWrite(SSR_Left, LOW);
        }
        else
            prevMillRun = millis();
    }
    else
    {
        digitalWrite(SSR_Right, LOW);
        digitalWrite(SSR_Left, LOW);
    }
}
CRGB color;

void LED::update(float linear_x, float angular_z, bool chan3)
{
    if (chan3)
    {
        if (linear_x == 0.0 && angular_z == 0.0)
        {
            color.r = 255, color.g = 0, color.b = 0;
        }
        else
        {
            color.r = 0, color.g = 0, color.b = 255;
        }
        this->fade(color);
    }
    else
    {
        this->Clear();
    }

    digitalWrite(SSR_Brake, (linear_x == 0.0 && angular_z == 0.0));

    this->steering(angular_z);
}

LED::~LED()
{
}