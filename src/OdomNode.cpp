#include "OdomNode.hpp"

OdomNode::OdomNode(/* args */)
{
  this->_ODrive = new ODrive;
}

void OdomNode::GetVelocity()
{
  if (millis() - this->getVelMillis > 16)
  {
    // Save Previous Position
    this->_Global_Right_Motor_Position_Prev = this->_Global_Right_Motor_Position;
    this->_Global_Left_Motor_Position_Prev = this->_Global_Left_Motor_Position;
    // Read Motor Velocity
    this->_Global_Right_Motor_Speed = this->_ODrive->odrive->GetVelocity(RightMotor);
    this->_Global_Left_Motor_Speed = -1 * this->_ODrive->odrive->GetVelocity(LeftMotor);
    // Read Motor Position
    this->_Global_Right_Motor_Position = this->_ODrive->odrive->GetPosition(RightMotor);
    this->_Global_Left_Motor_Position = -1 * this->_ODrive->odrive->GetPosition(LeftMotor);

    getVelMillis = millis();
  }
}

const void OdomNode::euler_to_quat(float x, float y, float z, float *q)
{
  float c1 = cos((y) / 2);
  float c2 = cos((z) / 2);
  float c3 = cos((x) / 2);

  float s1 = sin((y) / 2);
  float s2 = sin((z) / 2);
  float s3 = sin((x) / 2);

  q[0] = sin(x / 2) * cos(y / 2) * cos(z / 2) - cos(x / 2) * sin(y / 2) * sin(z / 2);
  q[1] = cos(x / 2) * sin(y / 2) * cos(z / 2) + sin(x / 2) * cos(y / 2) * sin(z / 2);
  q[2] = cos(x / 2) * cos(y / 2) * sin(z / 2) - sin(x / 2) * sin(y / 2) * cos(z / 2);
  q[3] = cos(x / 2) * cos(y / 2) * cos(z / 2) + sin(x / 2) * sin(y / 2) * sin(z / 2);
}

/**
 * @brief odometry callback function.
 * it calculates the odometry traveled by the robot,
 * and publish it as odometry topic.
 *
 * @return nav_msgs__msg__Odometry
 */
nav_msgs__msg__Odometry OdomNode::callback()
{
  float tyre_circumference = 2 * 3.14159 * WheelRadius;
  
  float linear_x = tyre_circumference * (this->_Global_Left_Motor_Speed + this->_Global_Right_Motor_Speed) / (2.0);
  float angular_z = tyre_circumference * (this->_Global_Right_Motor_Speed - this->_Global_Left_Motor_Speed) / (WheelBase);

  this->odom_msg.twist.twist.linear.x = linear_x;
  this->odom_msg.twist.twist.angular.z = angular_z;

  // position in meters
  float position_R_meter = this->_Global_Right_Motor_Position * tyre_circumference;
  float position_L_meter = this->_Global_Left_Motor_Position * tyre_circumference;

  float position_R_meter_Prev = this->_Global_Right_Motor_Position_Prev * tyre_circumference;
  float position_L_meter_Prev = this->_Global_Left_Motor_Position_Prev * tyre_circumference;

  // #Distance travelled
  float delta_pos_l_m = position_L_meter - position_L_meter_Prev;
  float delta_pos_r_m = position_R_meter - position_R_meter_Prev;
  
  float d = (delta_pos_l_m + delta_pos_r_m) / 2.0;        // delta_ps
  float th = (delta_pos_r_m - delta_pos_l_m) / WheelBase; // works for small angles

  float xd = cos(th) * d;
  float yd = -sin(th) * d;

  this->_x += cos(this->_theta) * xd - sin(this->_theta) * yd;
  this->_y += sin(this->_theta) * xd + cos(this->_theta) * yd;
  this->_theta = (this->_theta + th);
  if (this->_theta > 2 * 3.14159)
    this->_theta = 0;

  this->odom_msg.pose.pose.position.x = this->_x;
  this->odom_msg.pose.pose.position.y = this->_y;

  float q[4];
  euler_to_quat(0, 0, this->_theta, q);

  this->odom_msg.pose.pose.orientation.z = q[2];
  this->odom_msg.pose.pose.orientation.w = q[3];

  return this->odom_msg;
}
OdomNode::~OdomNode()
{
}
