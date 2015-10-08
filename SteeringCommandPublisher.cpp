#include "SteeringCommandPublisher.h"
#include <math.h>
#include <unistd.h>

#define UNSUPPORTED_MSG_TYPE -1
#define STEERING_MSG_TYPE 0
#define ACCEL_MSG_TYPE 1
#define BRAKE_MSG_TYPE 2
#define STEERING_LEFT_BUTTON_MSG_TYPE 3
#define STEERING_RIGHT_BUTTON_MSG_TYPE 4
#define ACCEL_BUTTON_MSG_TYPE 5
#define BRAKE_BUTTON_MSG_TYPE 6
#define STEERING_AXIS 0
#define ACCEL_AXIS 1
#define BRAKE_AXIS 2
#define MAX_STEERING_ANGLE 0.401425728
#define THROTTLE_MULTIPLIER 1.0
#define BRAKE_MULTIPLIER 1.0
#define STEERING_BUTTON_STEP_FACTOR 1000

using namespace std;
using namespace drake;

int8_t SteeringCommandPublisher::get_message_type(device_event const & event) const
{
  int8_t msg_type = UNSUPPORTED_MSG_TYPE;
  
  if (m_type == JOYSTICK) {
    if (event.type == JS_EVENT_AXIS) {
      switch (event.number) {
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
  } else {
    if (event.type == EV_KEY) {
      switch (event.number) {
       case KEY_UP:
        msg_type = ACCEL_BUTTON_MSG_TYPE;
        break;
       case KEY_DOWN:
        msg_type = BRAKE_BUTTON_MSG_TYPE;
        break;
       case KEY_LEFT:
        msg_type = STEERING_LEFT_BUTTON_MSG_TYPE;
        break;
       case KEY_RIGHT:
        msg_type = STEERING_RIGHT_BUTTON_MSG_TYPE;
        break;
      }
    }
  }

  return msg_type;
}

void SteeringCommandPublisher::set_message_value(device_event const & event, const int8_t msg_type, lcmt_driving_control_cmd_t & msg) const {
  if (m_type == JOYSTICK) {
    static const double scaleFactor = static_cast<double>((1<<15) - 1);
    switch (msg_type) {
      case STEERING_MSG_TYPE:
        msg.steering_angle = -event.value / scaleFactor * MAX_STEERING_ANGLE;
        printf("steering: %lf\n", msg.steering_angle);
        break;
      case ACCEL_MSG_TYPE:
        msg.throttle_value = THROTTLE_MULTIPLIER*(-0.5*event.value / scaleFactor + 0.5);
        printf("throttle: %lf\n", msg.throttle_value);
        break;
      case BRAKE_MSG_TYPE:
        msg.brake_value = BRAKE_MULTIPLIER*(-0.5*event.value / scaleFactor + 0.5);
        printf("brake: %lf\n", msg.brake_value);
        break;
    }
  } else {
    set_message_value(msg);
  }
}

void SteeringCommandPublisher::set_message_value(lcmt_driving_control_cmd_t& msg) const {
  if (m_type == KEYBOARD) {
    msg.steering_angle = min(MAX_STEERING_ANGLE / STEERING_BUTTON_STEP_FACTOR * m_keyboard_state.left_key + msg.steering_angle, MAX_STEERING_ANGLE);
    msg.steering_angle = max(MAX_STEERING_ANGLE / -STEERING_BUTTON_STEP_FACTOR * m_keyboard_state.right_key + msg.steering_angle, -1 * MAX_STEERING_ANGLE);
    msg.throttle_value = THROTTLE_MULTIPLIER * m_keyboard_state.up_key;
    msg.brake_value = BRAKE_MULTIPLIER * m_keyboard_state.down_key;
  }
}

void SteeringCommandPublisher::update_keyboard_state(device_event const& event, const int8_t msg_type) {
  switch(msg_type) {
    case STEERING_LEFT_BUTTON_MSG_TYPE:
      m_keyboard_state.left_key = event.value > 0;
      break;
    case STEERING_RIGHT_BUTTON_MSG_TYPE:
      m_keyboard_state.right_key = event.value > 0;
      break;
    case ACCEL_BUTTON_MSG_TYPE:
      m_keyboard_state.up_key = event.value > 0;
      break;
    case BRAKE_BUTTON_MSG_TYPE:
      m_keyboard_state.down_key = event.value > 0;
      break;
  }
}

SteeringCommandPublisher::SteeringCommandPublisher(string const & device_name, string const & channel_name)
: m_channel_name(channel_name) {
  if (device_name.find("js") != string::npos) {
    m_device = new Joystick(device_name);
    m_type = JOYSTICK;
  } else {
    m_device = new Keyboard(device_name);
    m_type = KEYBOARD;
  }
}

bool SteeringCommandPublisher::build_message(device_event const & event, drake::lcmt_driving_control_cmd_t & lcmt_driving_control_cmd) {
  int8_t msg_type = get_message_type(event);
  if (msg_type != UNSUPPORTED_MSG_TYPE) {
    lcmt_driving_control_cmd.timestamp = event.time;
    if (m_type == KEYBOARD) update_keyboard_state(event, msg_type);
    set_message_value(event, msg_type, lcmt_driving_control_cmd);
  } else {
    printf("Unpublished Event: time %8u, value %8hd, type: %3u, axis/button: %u\n", event.time, event.value, event.type, event.number);
  }

  return msg_type != UNSUPPORTED_MSG_TYPE;
}

void SteeringCommandPublisher::publish()
{
  device_event event;
  int rc = m_device->read_device_event(event);
  if (rc == DEVICE_SUCCESS) {
    if (build_message(event, m_last_message)) {
      m_lcm.publish(m_channel_name, &m_last_message);
    }
  } else if (rc == 0 && m_type == KEYBOARD) {
    set_message_value(m_last_message);
    m_lcm.publish(m_channel_name, &m_last_message);
    usleep(1000);
  }
}

bool SteeringCommandPublisher::good() const
{
  return m_lcm.good() && m_device->is_open();
}
