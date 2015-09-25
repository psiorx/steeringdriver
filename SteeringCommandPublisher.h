#ifndef STEERING_COMMAND_PUBLISHER_H
#define STEERING_COMMAND_PUBLISHER_H

#include <string>
#include <lcm/lcm-cpp.hpp>
#include "lcmtypes/drake/lcmt_driving_control_cmd_t.hpp"

#include "Joystick.h"
#include "Keyboard.h"

enum Type {
  JOYSTICK = 0,
  KEYBOARD = 1
};

struct keyboard_state {
  bool up_key = false;
  bool down_key = false;
  bool left_key = false;
  bool right_key = false;
};

class SteeringCommandPublisher
{
public:
  // For Joystick Control
  SteeringCommandPublisher(std::string const & device_name, std::string const & channel_name);
  // For Keyboard Control
  SteeringCommandPublisher(std::string const& channel_name);
  void publish();
  bool good() const;
private:
  bool build_message(device_event const & event, drake::lcmt_driving_control_cmd_t & msg);
  int8_t get_message_type(device_event const & event) const;
  void set_message_value(device_event const & event, const int8_t msg_type, drake::lcmt_driving_control_cmd_t & lcmt_driving_control_cmd) const;
  void set_message_value(drake::lcmt_driving_control_cmd_t& lcmt_driving_control_cmd) const;
  void update_keyboard_state(device_event const& event, const int8_t msg_type);
  Device* m_device;
  Type m_type;
  lcm::LCM m_lcm;
  std::string m_channel_name;
  drake::lcmt_driving_control_cmd_t m_last_message;
  keyboard_state m_keyboard_state;
};

#endif
