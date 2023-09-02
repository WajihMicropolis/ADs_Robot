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
CRGB color;
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

  //! 2. Read Ultrasonic

  Ultrasonic.update();
  Ultrasonic.control(Linear_x, Angular_z);

  ODRIVE.SetSpeed(Linear_x, Angular_z);

  if (RC.Data.Chan3)
  {
    if (Linear_x == 0.0 && Angular_z == 0.0)
    {
      color.r = 255, color.g = 0, color.b = 0;
    }
    else
    {
      color.r = 0, color.g = 0, color.b = 255;
    }
    led.fade(color);
  }
  else
  {
    led.Clear();
  }
  led.steering(Angular_z);

  // delay(1);
  // Serial.println("-------------");
}