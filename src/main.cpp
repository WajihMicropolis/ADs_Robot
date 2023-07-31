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

  RC.Init();
  ODRIVE.Init();
  // ROS.Init();
}

void loop()
{
  if (RC.checkFailSafe(*RC.steering))
  {
    Serial.println("RC NOT CONNECTED!!!");
  }
  else
  {
    Serial.println("RC CONNECTED");
    RC.getVal(RC.Data, true);
  }

  ODRIVE.SetSpeed(RC.Data.Throttle, RC.Data.Steering);
  // ROS.Update();

  // Serial.println(RC.Data.Steering);
}
// -1 -100 0 0

// 0 0 15 0