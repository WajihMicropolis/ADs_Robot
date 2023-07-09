#include "RC_Control.hpp"

void RC_Control::Init()
{
    // create objects with the corresponding pin
    steering = new PWM(5);
    throttle = new PWM(18);
    // channel3 = new PWM(18);
    // channel4 = new PWM(18);

    steering->begin(true);
    throttle->begin(true);
    // channel3->begin(true);
    // channel4->begin(true);

    Serial.println("[RC_CONTROL] RC Initialized");
}

int RC_Control::RC_Read_PWM(PWM &channel)
{
    int val = channel.getValue();
    val = constrain(val, 1000, 2000);
    int mapped_val = map(val, 1000, 2000, -100, 100);

    return mapped_val;
}

bool RC_Control::RC_Read_Bool(PWM &channel)
{
    //! To be re-coded
    // bool x = &channel == channel3;
    this->prev_val = this->current_val;
    this->current_val = channel.getValue();
    if (this->current_val < 2000 && this->prev_val > 2000)
    {
        this->buttonCh = !this->buttonCh;
    }
    return this->buttonCh;
}

void RC_Control::getVal(RCval &RC, bool debug)
{
    RC.Steering = RC_Read_PWM(*steering);
    RC.Throttle = RC_Read_PWM(*throttle);
    // RC.Chan3 =    RC_Read_PWM(*channel3);
    // RC.Chan4 = RC_Read_PWM(*channel4);

    if (!debug)
        return;
    Serial.printf("_steeringVal: %d \t", RC.Steering);
    Serial.printf("_throttleVal: %d \t", RC.Throttle);
}

RC_Control::~RC_Control()
{
}
RC_Control::RC_Control()
{
}