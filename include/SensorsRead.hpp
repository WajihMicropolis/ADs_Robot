#ifndef _SENSORS_READ_HPP_
#define _SENSORS_READ_HPP_

#include <Arduino.h>

class SensorsRead
{
private:
    struct sensor
    {
        uint8_t trigPin;
        uint8_t echoPin;

        sensor(uint8_t _trigPin, uint8_t _echoPin)
        { // constructor
            trigPin = _trigPin;
            echoPin = _echoPin;
        }

        void setPinMode()
        { // setting pinMode
            pinMode(trigPin, OUTPUT);
            pinMode(echoPin, INPUT);
        }

        float getDistance()
        { // calculating distance
            digitalWrite(trigPin, LOW);
            delayMicroseconds(2);
            digitalWrite(trigPin, HIGH);
            delayMicroseconds(10);
            digitalWrite(trigPin, LOW);
            delayMicroseconds(2);
            float duration = pulseIn(echoPin, HIGH);
            float distance = duration * 0.034 / 2;

            return int(distance);
        }
    };
    sensor *right_ultrasonic,
        *center_right_ultrasonic,
        *center_left_ultrasonic,
        *left_ultrasonic;

    uint64_t prevMill = 0;
    uint8_t readDuration = 100;
#define left_trig 26
#define left_echo 25

#define center_left_trig 13
#define center_left_echo 14

#define center_right_trig 22
#define center_right_echo 23

#define right_trig 21
#define right_echo 19

public:
    void Init();
    void update(bool debug = LOW);
    void control(float &linear_x, float &angular_z,  bool debug = false);
    float rightDist,
        centRightDist,
        centLeftDist,
        leftDist;
    SensorsRead();
    ~SensorsRead();
};

#endif //_SENSORS_READ_HPP_