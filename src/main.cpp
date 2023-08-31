#include <Arduino.h>
#include "RC_Control.hpp"
#include "ODrive.hpp"
#include "RosNode.hpp"
#include "SensorsRead.hpp"

// RC_Control RC;
// ODrive ODRIVE;
uROS ROS;

void setup()
{
  Serial.begin(115200);
  // sensorsInit();
  // RC.Init();
  // ODRIVE.Init();
  ROS.Init();
}

void loop()
{
  // updateSensors();

  ROS.Update();
  /*if (!ROS.rosNodeAvail)
  {
    // Serial.println("ROS NODE NOTTTTTT Avail");

    if (RC.checkFailSafe(*RC.steering))
    {
      // Serial.println("RC NOT CONNECTED!!!");
      RC.Data.Throttle = RC.Data.Steering = 0.0;
    }
    else
    {
      // Serial.println("RC CONNECTED");
      RC.getVal(RC.Data,true);

    }
    ODRIVE.SetSpeed(RC.Data.Throttle, RC.Data.Steering);
  }
  else
  {
    // Serial.println("ROS NODE Avail");
    ODRIVE.SetSpeed(ROS.cmdVel.linear.x, ROS.cmdVel.angular.z);
  }
delay(50);*/
  // Serial.println("-------------");
}