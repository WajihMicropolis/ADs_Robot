#include "SensorsRead.hpp"

SensorsRead::SensorsRead(/* args */)
{

    right_ultrasonic = new sensor(right_trig, right_echo);
    center_right_ultrasonic = new sensor(center_right_trig, center_right_echo);
    center_left_ultrasonic = new sensor(center_left_trig, center_left_echo);
    left_ultrasonic = new sensor(left_trig, left_echo);
}

void SensorsRead::Init()
{
    // initialize 4 objects for ultrasonic struct
    this->right_ultrasonic->setPinMode();
    this->center_right_ultrasonic->setPinMode();
    this->center_left_ultrasonic->setPinMode();
    this->left_ultrasonic->setPinMode();
}

void SensorsRead::update(bool debug)
{
    if (millis() - prevMill > readDuration)
    {
        rightDist = this->right_ultrasonic->getDistance();
        centRightDist = this->center_right_ultrasonic->getDistance();
        centLeftDist = this->center_left_ultrasonic->getDistance();
        leftDist = this->left_ultrasonic->getDistance();
        if (debug)
        {
            Serial.printf("right: %f \n", rightDist);
            Serial.printf("center_right: %f \n", centRightDist);
            Serial.printf("center_left: %f \n", centLeftDist);
            Serial.printf("left: %f \n", leftDist);
            Serial.println("--------------");
        }
        prevMill = millis();
    }
}

void SensorsRead::control(float &linear_x, float &angular_z, bool debug)
{
    float old_lin_x = linear_x,
          old_ang_z = angular_z;

    if ((this->rightDist < 15) && (this->leftDist < 15) || (this->centRightDist < 15) && (this->centLeftDist < 15))
    { //! FULL STOP
        linear_x = linear_x > 0.0 ? 0 : linear_x;
        angular_z = angular_z;

        // TODO RED LED ON
    }
    else if (this->rightDist < 15 || (this->centRightDist < 15))
    { //! Turn Left
        linear_x = linear_x / 2;

        // angular_z = 0.5;
        angular_z = angular_z > 0.0 ? angular_z : 0;
    }
    else if ((this->leftDist < 15) || (this->centLeftDist < 15))
    { //! Turn Right
        linear_x = linear_x / 2;

        // angular_z = -0.5;
        angular_z = angular_z < 0.0 ? angular_z : 0;
    }
    else
    {
        linear_x = linear_x,
        angular_z = angular_z;
    }

    if (debug)
    {
        Serial.printf("old linear_x: %f \t", old_lin_x);
        Serial.printf("old angular_z: %f \t", old_ang_z);
        Serial.printf("after linear_x: %f \t", linear_x);
        Serial.printf("after angular_z: %f \n", angular_z);
    }
}

SensorsRead::~SensorsRead()
{
}