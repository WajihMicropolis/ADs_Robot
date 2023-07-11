#include "MicroROS.hpp"


void CmdVelCb(const void *msgin)
{
  const geometry_msgs__msg__Twist *msg = (const geometry_msgs__msg__Twist *)msgin;

//   Linear_Angle_Speed_Conv(msg->linear.x, msg->angular.z);

  float led_intensity = map(abs(msg->linear.x), 0, 1, 0, 255);
//   analogWrite(LED_PIN, led_intensity);
}

bool uROS::create_entities()
{
  // Initialize micro-ROS allocator
  // The allocator object wraps the dynamic memory allocation and deallocating methods used in micro-ROS
  allocator = rcl_get_default_allocator();

  // create init_options
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

  // create node
  const char *node_name = "Sensor_Board";

  RCCHECK(rclc_node_init_default(&node, node_name, "", &support));

  // create int32_publisher
  const char *sub_topic_name = "cmd_vel";

  // create subscriber
  RCCHECK(rclc_subscription_init_default(
      &cmd_vel_subscriber,
      &node,
      ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist),
      sub_topic_name));

  // create executor
  executor = rclc_executor_get_zero_initialized_executor();

  RCCHECK(rclc_executor_init(&executor, &support.context, 4, &allocator));

  RCCHECK(rclc_executor_add_subscription(&executor, &cmd_vel_subscriber, &cmd_vel, &CmdVelCb, ON_NEW_DATA));

  return true;
}

void uROS::destroy_entities()
{
  rmw_context_t *rmw_context = rcl_context_get_rmw_context(&support.context);
  (void)rmw_uros_set_context_entity_destroy_session_timeout(rmw_context, 0);

  bool destroy_sub = rcl_subscription_fini(&this->cmd_vel_subscriber, &this->node);

  bool destroy_timer = rcl_timer_fini(&timer);
  bool destroy_node = rcl_node_fini(&this->node);
  rclc_executor_fini(&executor);
  rclc_support_fini(&support);
}

void uROS::ros_init()
{
  // Configure serial transport
  // pinMode(LED_PIN, OUTPUT);
  // digitalWrite(LED_PIN, LOW);

  set_microros_serial_transports(Serial);
  AgentState = WAITING_AGENT;
}

void uROS::ros_loop()
{
  switch (AgentState)
  {

  case WAITING_AGENT:
    EXECUTE_EVERY_N_MS(500, AgentState = (RMW_RET_OK == rmw_uros_ping_agent(100, 1)) ? AGENT_AVAILABLE : WAITING_AGENT;);

    break;

  case AGENT_AVAILABLE:
    // create_entities function re-initialize micro-ros setup
    AgentState = (true == create_entities()) ? AGENT_CONNECTED : WAITING_AGENT;
    if (AgentState == WAITING_AGENT)
    {
      destroy_entities();
      // digitalWrite(13, LOW);
    };

    break;

  case AGENT_CONNECTED:
    EXECUTE_EVERY_N_MS(200, AgentState = (RMW_RET_OK == rmw_uros_ping_agent(100, 1)) ? AGENT_CONNECTED : AGENT_DISCONNECTED;);
    if (AgentState == AGENT_CONNECTED)
    {
      RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
      RCCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
    }

    break;

  case AGENT_DISCONNECTED:
    destroy_entities();
    AgentState = WAITING_AGENT;
    // analogWrite(13, 0);
    // Linear_Angle_Speed_Conv(0, 0);

    break;

  default:
    break;
  }
}
uROS::uROS(/* args */)
{
}

uROS::~uROS()
{
}