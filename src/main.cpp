#include <Arduino.h>
#include "RC_Control.hpp"
#include "ODrive.hpp"
#include "MicroROS.hpp"

RC_Control RC;
ODrive ODRIVE;

void setup()
{
  Serial.begin(115200); 

  RC.Init();
  ODRIVE.Init();

}

void loop()
{

  RC.getVal(RC.Val);

  ODRIVE.motorControl(RC.Val.Throttle, RC.Val.Steering);
  ODRIVE.GetVelocity();

  // Serial.println(RC.Val.Steering);
}