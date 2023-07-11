#ifndef _MICRO_ROS_HPP_
#define _MICRO_ROS_HPP_

#include <Arduino.h>
#include <micro_ros_platformio.h>
#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <rmw_microros/rmw_microros.h>
#include <rcl_interfaces/msg/log.h>
#include <geometry_msgs/msg/twist.h>
#include <geometry_msgs/msg/pose.h>
#include <nav_msgs/msg/odometry.h>


#include "ODrive.hpp"

class uROS
{
private:
#define RCCHECK(fn)                  \
    {                                \
        rcl_ret_t temp_rc = fn;      \
        if ((temp_rc != RCL_RET_OK)) \
        {                            \
        }                            \
    }
#define RCSOFTCHECK(fn)              \
    {                                \
        rcl_ret_t temp_rc = fn;      \
        if ((temp_rc != RCL_RET_OK)) \
        {                            \
        }                            \
    }

#define EXECUTE_EVERY_N_MS(MS, X)          \
                                           \
    do                                     \
    {                                      \
        static volatile int64_t init = -1; \
        if (init == -1)                    \
        {                                  \
            init = uxr_millis();           \
        }                                  \
        if (uxr_millis() - init > MS)      \
        {                                  \
            X;                             \
            init = uxr_millis();           \
        }                                  \
    } while (0);

    rcl_subscription_t cmd_vel_subscriber;
    geometry_msgs__msg__Twist cmd_vel;

    rcl_publisher_t odom_publisher;
    nav_msgs__msg__Odometry odom;

    rcl_node_t node;
    rclc_support_t support;
    rclc_executor_t executor;
    rcl_allocator_t allocator;

    rcl_timer_t timer;
    rcl_timer_t timer2;

    bool _micro_ros_init_successful;

    float _x = 0,
          _y = 0,
          theta = 0;

    enum states
    {
        WAITING_AGENT,
        AGENT_AVAILABLE,
        AGENT_CONNECTED,
        AGENT_DISCONNECTED
    } AgentState;

    const void euler_to_quat(float x, float y, float z, float *q);
    // void odom_cb(rcl_timer_t *timer, int64_t last_call_time);
    bool create_entities();
    void destroy_entities();

public:
    void ros_init();
    void ros_loop();

    uROS(/* args */);
    ~uROS();
};

#endif // _MICRO_ROS_HPP_