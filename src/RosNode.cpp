#include "RosNode.hpp"

rcl_subscription_t _Cmd_vel_subscriber;
geometry_msgs__msg__Twist _Cmd_vel;
const geometry_msgs__msg__Twist *cmdVelMsg;

rcl_publisher_t _Pose_Pub;
geometry_msgs__msg__Pose Pose_msg;

rcl_publisher_t _Twist_Pub;
geometry_msgs__msg__Twist Twist_msg;

nav_msgs__msg__Odometry odom_msg;
float uROSLinear_x = 0,
      uROSAngular_z = 0;

void uROS::Init()
{
   pinMode(2, OUTPUT);
   set_microros_serial_transports(Serial);
   AgentState = WAITING_AGENT;
}

void poseCb(rcl_timer_t *timer, int64_t last_call_time)
{
   RCLC_UNUSED(last_call_time);
   RCCHECK(rcl_publish(&_Pose_Pub, &Pose_msg, NULL));
}

void twistCb(rcl_timer_t *timer, int64_t last_call_time)
{
   RCLC_UNUSED(last_call_time);
   RCCHECK(rcl_publish(&_Twist_Pub, &Twist_msg, NULL));
}

void CmdVelCb(const void *msgin)
{
   cmdVelMsg = (const geometry_msgs__msg__Twist *)msgin;
   uROSLinear_x = cmdVelMsg->linear.x;
   uROSAngular_z = cmdVelMsg->angular.z;
}

bool uROS::create_entities()
{
   const char *node_name = "uController";
   // Initialize micro-ROS allocator
   allocator = rcl_get_default_allocator();
   // create init_options
   RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));
   // create node
   RCCHECK(rclc_node_init_default(&node, node_name, "", &support));

   // Topic Names
   const char *cmdVel_topic = "cmd_vel";
   const char *pose_topic = "pose";
   const char *twist_topic = "twist";
   // create subscriber
   RCCHECK(rclc_subscription_init_default(
       &_Cmd_vel_subscriber,
       &node,
       ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist),
       cmdVel_topic));

   RCCHECK(rclc_publisher_init_default(
       &_Pose_Pub,
       &node,
       ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Pose), // message type
       pose_topic));

   RCCHECK(rclc_publisher_init_default(
       &_Twist_Pub,
       &node,
       ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist), // message type
       twist_topic));

   // create timer,
   const unsigned int timer_timeout = 10;

   RCCHECK(rclc_timer_init_default(
       &_Pose_timer,
       &support,
       RCL_MS_TO_NS(timer_timeout),
       poseCb));

   RCCHECK(rclc_timer_init_default(
       &_Twist_timer,
       &support,
       RCL_MS_TO_NS(timer_timeout),
       twistCb));

   // create executor
   executor = rclc_executor_get_zero_initialized_executor();
   RCCHECK(rclc_executor_init(&executor, &support.context, 3, &allocator));
   RCCHECK(rclc_executor_add_timer(&executor, &_Pose_timer));  // used for publisher
   RCCHECK(rclc_executor_add_timer(&executor, &_Twist_timer)); // used for publisher

   RCCHECK(rclc_executor_add_subscription(&executor, &_Cmd_vel_subscriber, &_Cmd_vel, &CmdVelCb, ON_NEW_DATA));
   return true;
}
void uROS::destroy_entities()
{
   rmw_context_t *rmw_context = rcl_context_get_rmw_context(&support.context);
   (void)rmw_uros_set_context_entity_destroy_session_timeout(rmw_context, 0);

   bool destroy_sub = rcl_subscription_fini(&_Cmd_vel_subscriber, &node);
   bool destroy_timer = rcl_timer_fini(&_Pose_timer);
   bool destroy_timer_2 = rcl_timer_fini(&_Twist_timer);
   bool destroy_node = rcl_node_fini(&node);
   rclc_executor_fini(&executor);
   rclc_support_fini(&support);
}

std::pair<float, float> uROS::Update()
{

   switch (AgentState)
   {
   case WAITING_AGENT:
      EXECUTE_EVERY_N_MS(500, AgentState = (RMW_RET_OK == rmw_uros_ping_agent(100, 1)) ? AGENT_AVAILABLE : WAITING_AGENT;);
      break;

   case AGENT_AVAILABLE: // create_entities function re-initialize micro-ros setup
      AgentState = (create_entities() == true) ? AGENT_CONNECTED : WAITING_AGENT;
      if (AgentState == WAITING_AGENT)
      {
         // digitalWrite(2, !digitalRead(2));
         destroy_entities();
      }
      break;

   case AGENT_CONNECTED:
      EXECUTE_EVERY_N_MS(200, AgentState = (RMW_RET_OK == rmw_uros_ping_agent(100, 1)) ? AGENT_CONNECTED : AGENT_DISCONNECTED;);
      if (AgentState == AGENT_CONNECTED)
      {
         this->rosNodeAvail = true;

         this->_odom->update();
         odom_msg = this->_odom->callback(true);

         Twist_msg = odom_msg.twist.twist;
         Pose_msg = odom_msg.pose.pose;
         Serial.println("Pose_msg x:" + String(Pose_msg.position.x));
         Serial.println("Pose_msg y:" + String(Pose_msg.position.y));

         RCCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
         return {uROSLinear_x, uROSAngular_z};
      }
      break;

   case AGENT_DISCONNECTED:
      this->rosNodeAvail = false;
      destroy_entities();
      AgentState = WAITING_AGENT;
      break;

   default:
      break;
   }
   return {0, 0};
}

uROS::uROS()
{
   this->_odom = new OdomNode;
}

uROS::~uROS()
{
}