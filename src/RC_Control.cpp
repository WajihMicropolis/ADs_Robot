#include "RC_Control.hpp"

void RC_Control::Init()
{
    // create objects with the corresponding pin
    steering = new PWM(steeringPin);
    throttle = new PWM(throttlePin);
    channel3 = new PWM(chan3Pin);
    // channel4 = new PWM(18);

    steering->begin(true);
    throttle->begin(true);
    channel3->begin(true);
    // channel4->begin(true);

    Serial.println("[RC_CONTROL] RC Initialized");
}

int RC_Control::RC_Read_PWM(PWM &channel)
{
    int val = channel.getValue();
    val = constrain(val, 1000, 2000);
    int mapped_val = map(val, 1000, 2000, -100, 100);

    if (mapped_val > -10 && mapped_val < 10)
        mapped_val = 0;

    return mapped_val;
}

bool RC_Control::checkFailSafe(PWM &channel)
{
    //! Check for FAILSAFE
    this->_Safe = channel.getValue();
    if (this->_Safe == this->_prevSafe)
    {
        if (millis() - prevmil > 1000) // if the value is fixed for 1000 ms so the RC is OFF
        {
            failSafe = HIGH;
        }
    }
    else
    {
        failSafe = LOW;
        prevmil = millis();
    }
    this->_prevSafe = this->_Safe;
    return failSafe;
}
void RC_Control::getVal(RCData &RC, bool debug)
{
    RC.Steering = RC_Read_PWM(*steering);
    RC.Steering = RC.Steering / 100;
    RC.Throttle = RC_Read_PWM(*throttle);
    RC.Throttle = RC.Throttle / 100;
    RC.Chan3 = RC_Read_PWM(*channel3) > 0 ? HIGH : LOW;
    // RC.Chan4 = RC_Read_PWM(*channel4);

    if (!debug)
        return;
    Serial.printf("SteeringVal: %f \t", RC.Steering);
    Serial.printf("failSafe: %d \t", failSafe);
    Serial.printf("ThrottleVal: %f \t", RC.Throttle);
    Serial.printf("Channel3Val: %d \n", RC.Chan3);
}

RC_Control::~RC_Control()
{
}
RC_Control::RC_Control()
{
}