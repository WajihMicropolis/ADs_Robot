#include <Arduino.h>
#include "RC_Control.hpp"
#include "ODrive.hpp"
#include "RosNode.hpp"

RC_Control RC;

uROS ROS;
void setup()
{
  Serial.begin(115200);

  RC.Init();
  ROS.Init();
}

void loop()
{
  RC.getVal(RC.Val);
  
  ROS.Update();

  // Serial.println(RC.Val.Steering);
}