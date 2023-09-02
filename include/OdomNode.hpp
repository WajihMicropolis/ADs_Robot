#ifndef _ODOM_NODE_HPP_
#define _ODOM_NODE_HPP_

#include <Arduino.h>
#include "ODrive.hpp"
#include <geometry_msgs/msg/twist.h>
#include <geometry_msgs/msg/pose.h>
#include <nav_msgs/msg/odometry.h>
class OdomNode
{
private:
    float _x = 0,
          _y = 0,
          _theta = 0;
    const void euler_to_quat(float x, float y, float z, float *q);

    float _Global_Right_Motor_Position,
        _Global_Left_Motor_Position,
        _Global_Right_Motor_Position_Prev,
        _Global_Left_Motor_Position_Prev,
        _Global_Right_Motor_Speed,
        _Global_Left_Motor_Speed;

    unsigned long getVelMillis = 0;
    nav_msgs__msg__Odometry odom_msg;
    ODrive *_ODrive;

public:
    void update(bool debug = false);
    geometry_msgs__msg__Pose updatePose();
    geometry_msgs__msg__Twist updateTwist();
    nav_msgs__msg__Odometry callback(bool debug);

    // rcl_publisher_t odom_publisher;
    OdomNode();
    ~OdomNode();
};

#endif //_ODOM_NODE_HPP_
