#ifndef _SENSORS_READ_HPP_
#define _SENSORS_READ_HPP_

#include <Arduino.h>

#define numOfUs 4
#define left_trig 22
#define left_echo 23

const int trigPins[numOfUs] = {22, 0, 0, 0};
const int echoPins[numOfUs] = {23, 0, 0, 0};

void sensorsInit();
void updateSensors();

#endif //_SENSORS_READ_HPP_