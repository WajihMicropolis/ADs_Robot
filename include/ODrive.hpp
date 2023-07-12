#ifndef _ODRIVE_CONTROL_HPP_
#define _ODRIVE_CONTROL_HPP_

#include <ODriveArduino.h>
#include <Arduino.h>

#define ESP32_UART2_PIN_TX 17
#define ESP32_UART2_PIN_RX 16
#define BAUDRATE 115200

#define minThrPer 10
#define maxThrPer 100
#define minMotorSpeed 0
#define maxMotorFwSpeed 100

#define minSteerPer 10
#define maxSteerPer 100
#define maxSteerspeed 100

#define WheelRadius 0.158
#define WheelBase 0.3872

#define RightMotor 0
#define LeftMotor 1

class ODrive
{
private:
    float vel_limit = 1.0f,
          current_lim = 11.0f;

    float velocity0 = 0,
          velocity1 = 0;

    unsigned long getVelMillis = 0;

public:
    void Init();
    void motorControl(int RC_Throttle, int RC_Steering);
    void GetVelocity();
    void SetSpeed(float Linear_x, float Angle_z);
    void SerialControl();

    float Global_Right_Motor_Position,
        Global_Left_Motor_Position,
        Global_Right_Motor_Position_Prev,
        Global_Left_Motor_Position_Prev,
        Global_Right_Motor_Speed,
        Global_Left_Motor_Speed;

    ODrive(/* args */);
    ~ODrive();
};

// Printing with stream operator
template <class T>
inline Print &operator<<(Print &obj, T arg)
{
    obj.print(arg);
    return obj;
}
template <>
inline Print &operator<<(Print &obj, float arg)
{
    obj.print(arg, 4);
    return obj;
}

void ODrive_Init();
void ODriveSerialControl();

void ODriveRCControl(int RC_Throttle, int RC_Steering);
void ODriveSetSpeed(float Linear_x, float Angle_z);
void ODriveGetVelocity();

#endif //_ODRIVE_CONTROL_HPP_