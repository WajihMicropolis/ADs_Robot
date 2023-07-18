#include <Arduino.h>
#include "RC_Control.hpp"
#include "ODrive.hpp"
#include "RosNode.hpp"

RC_Control RC;
ODrive ODRIVE;
uROS ROS;
void setup()
{
  Serial.begin(115200);
  // RC.Init();
  // ODRIVE.Init();
  ROS.Init();
}

void loop()
{
  // RC.getVal(RC.Val);
  // ODRIVE.SetSpeed(RC.Val.Throttle, RC.Val.Steering);
  ROS.Update();

  // Serial.println(RC.Val.Steering);
}