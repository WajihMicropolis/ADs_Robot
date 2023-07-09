#ifndef _RC_CONTROL_HPP_
#define _RC_CONTROL_HPP_
#include <Arduino.h>
#include "PWM.hpp"

class RC_Control
{
private:
    PWM *steering;
    PWM *throttle;
    PWM *channel3;
    PWM *channel4;

    bool current_val = 0,
         prev_val = 0,
         buttonCh = LOW;

    int RC_Read_PWM(PWM &channel);
    bool RC_Read_Bool(PWM &channel);

public:
    struct RCval
    {
        int Steering;
        int Throttle;
        bool Chan3;
        bool Chan4;
    }Val;

    void Init();
    void getVal(RCval &RC,bool debug = false);

    RC_Control(/* args */);
    ~RC_Control();
};

#endif