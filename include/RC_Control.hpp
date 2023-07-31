#ifndef _RC_CONTROL_HPP_
#define _RC_CONTROL_HPP_
#include <Arduino.h>
#include "PWM.hpp"

class RC_Control
{
private:
    PWM *throttle;
    PWM *channel3;
    PWM *channel4;
#define steeringPin 35
#define throttlePin 34
#define chan3Pin 32

    bool current_val = 0,
         prev_val = 0,
         buttonCh = LOW;
    bool failSafe = LOW;

    int Button_val;
    float _Safe, _prevSafe;
    unsigned long prevmil = 0;

    int RC_Read_PWM(PWM &channel);
    bool RC_Read_Bool(PWM &channel);

public:
    PWM *steering;

    struct RCData
    {
        float Steering = 0.0;
        float Throttle = 0.0;
        bool Chan3 = LOW;
        bool Chan4 = LOW;
    } Data;

    void Init();
    void getVal(RCData &RC, bool debug = false);
    bool checkFailSafe(PWM &channel);

    RC_Control(/* args */);
    ~RC_Control();
};

#endif