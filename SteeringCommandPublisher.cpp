#include "SteeringCommandPublisher.h"
#include <math.h>

#define UNSUPPORTED_MSG_TYPE -1
#define STEERING_MSG_TYPE 0
#define ACCEL_MSG_TYPE 1
#define BRAKE_MSG_TYPE 2
#define STEERING_AXIS 0
#define ACCEL_AXIS 1
#define BRAKE_AXIS 2
#define MAX_STEERING_ANGLE 5.0*M_PI_2
#define THROTTLE_MULTIPLIER 0.5

using namespace std;
using namespace drake;

int8_t SteeringCommandPublisher::get_message_type(js_event const & jse) const
{
  int8_t msg_type = UNSUPPORTED_MSG_TYPE;
  
  if (jse.type == JS_EVENT_AXIS) {
    
    switch (jse.number) {
      case STEERING_AXIS:
        msg_type = STEERING_MSG_TYPE;
        break;
      case ACCEL_AXIS:
        msg_type = ACCEL_MSG_TYPE;
        break;
      case BRAKE_AXIS:
        msg_type = BRAKE_MSG_TYPE;
        break;
    }
  } 

  return msg_type;
}

void SteeringCommandPublisher::set_message_value(js_event const & jse, const int8_t msg_type, lcmt_driving_control_cmd_t & msg) const
{
  static const double scaleFactor = static_cast<double>((1<<15) - 1);
  switch (msg_type) {
    case STEERING_MSG_TYPE:
      msg.steering_angle = -jse.value / scaleFactor * MAX_STEERING_ANGLE;
      printf("steering: %lf\n", msg.steering_angle);
      break;
    case ACCEL_MSG_TYPE:
      msg.throttle_value = THROTTLE_MULTIPLIER*(-0.5*jse.value / scaleFactor + 0.5);
      printf("throttle: %lf\n", msg.throttle_value);
    break;
    case BRAKE_MSG_TYPE:
      msg.brake_value = -0.5*jse.value / scaleFactor + 0.5;
      printf("brake: %lf\n", msg.brake_value);
    break;
  }
}


SteeringCommandPublisher::SteeringCommandPublisher(string const & device_name, string const & channel_name)
: m_channel_name(channel_name), m_joystick(device_name) { }

bool SteeringCommandPublisher::build_message(js_event const & jse, drake::lcmt_driving_control_cmd_t & lcmt_driving_control_cmd) const
{
  int8_t msg_type = get_message_type(jse);
  if (msg_type != UNSUPPORTED_MSG_TYPE) {
    lcmt_driving_control_cmd.timestamp = jse.time;
    set_message_value(jse, msg_type, lcmt_driving_control_cmd);
  } else {
    printf("Unpublished Event: time %8u, value %8hd, type: %3u, axis/button: %u\n", jse.time, jse.value, jse.type, jse.number);
  }

  return msg_type != UNSUPPORTED_MSG_TYPE;
}

void SteeringCommandPublisher::publish()
{
  js_event jse;
  int rc = m_joystick.read_joystick_event(jse);
  if (rc == JOYSTICK_SUCCESS) {
    lcmt_driving_control_cmd_t lcmt_driving_control_cmd;
    if (build_message(jse, lcmt_driving_control_cmd)) {
      m_lcm.publish(m_channel_name, &lcmt_driving_control_cmd);
    }
  }
}

bool SteeringCommandPublisher::good() const
{
  return m_lcm.good() && m_joystick.is_open();
}
