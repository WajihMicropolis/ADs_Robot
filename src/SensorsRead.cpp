#include "SensorsRead.hpp"

uint64_t prevMill = 0;
uint8_t readDuration = 50;

struct SensorsRead
{
    int left;
    int right;
} us;



void sensorsInit()
{
    for (size_t i = 0; i < numOfUs; i++)
        pinMode(trigPins[i], OUTPUT), pinMode(echoPins[i], INPUT);
}

int read_distance(const uint8_t trig, const uint8_t echo)
{
    digitalWrite(trig, LOW);
    delayMicroseconds(5);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    delayMicroseconds(5);
    float duration = pulseIn(echo, HIGH, 100000);
    float distance = duration * 0.0342 / 2;

    return (distance < 401) ? (int)distance : 0;
}

void updateSensors()
{
    if (millis() - prevMill > readDuration)
    {

        // us.left = read_distance(left_trig, left_echo);
        // Serial.println(us.left), prevMill = millis();
    }
}