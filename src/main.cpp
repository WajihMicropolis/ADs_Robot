#include <Arduino.h>
#include "RC_Control.hpp"
#include "ODrive.hpp"
#include "RosNode.hpp"
#include "SensorsRead.hpp"
#include "Lights.hpp"

LED led;
RC_Control RC;
ODrive ODRIVE;
uROS microRos;
SensorsRead Ultrasonic;
std::pair<float, float> cmdVel(0, 0);

void setup()
{
  Serial.begin(115200);
  microRos.Init();
  RC.Init();
  ODRIVE.Init();
  Ultrasonic.Init();
}
float Linear_x = 0.0,
      Angular_z = 0.0;
void loop()
{
  //! 1. Get CmdVel

  cmdVel = microRos.Update();
  if (!microRos.rosNodeAvail) // uROS not available
  {
    if (RC.checkFailSafe(*RC.steering)) // RC not connected
    {
      Linear_x = Angular_z = 0.0;
    }
    else
    {
      RC.getVal(RC.Data);
      Linear_x = RC.Data.Throttle;
      Angular_z = RC.Data.Steering;
    }
  }
  else
  {
    Linear_x = cmdVel.first;
    Angular_z = cmdVel.second;
  }

  //! 2. Read Ultrasonic & Update Speed

  Ultrasonic.update();
  Ultrasonic.control(Linear_x, Angular_z);

  //! 3. Set Speed

  ODRIVE.SetSpeed(Linear_x, Angular_z);

  //! 4. Control Lights

  led.update(Linear_x, Angular_z, RC.Data.Chan3);

  // delay(1);
  // Serial.println("-------------");
}