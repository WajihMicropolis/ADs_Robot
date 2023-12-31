#ifndef _ROS_NODE_HPP_
#define _ROS_NODE_HPP_

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
#include <rcutils/logging_macros.h>
#include <rcl/time.h>
#include "OdomNode.hpp"

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

    bool _micro_ros_init_successful;
    rcl_node_t node;
    rclc_support_t support;
    rclc_executor_t executor;
    rcl_allocator_t allocator;

    rcl_timer_t _Pose_timer;
    rcl_timer_t _Twist_timer;

    OdomNode *_odom;

    enum states
    {
        WAITING_AGENT,
        AGENT_AVAILABLE,
        AGENT_CONNECTED,
        AGENT_DISCONNECTED
    } AgentState;

    bool create_entities();
    void destroy_entities();

public:
    // void getCmdVel();
    float linear_x,
        angular_z;
    bool rosNodeAvail = LOW;
    void Init();

    std::pair<float, float> Update();
    uROS(/* args */);
    ~uROS();
};

#endif // _ROS_NODE_HPP_