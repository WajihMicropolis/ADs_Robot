#include <Arduino.h>
#include "RC_Control.hpp"

RC_Control RC;

void setup()
{
  Serial.begin(115200); // Serial for debug

  RC.Init();
}

void loop()
{
  RC.getVal(RC.Val);


  Serial.println(RC.Val.Steering);
}