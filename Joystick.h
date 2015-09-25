#ifndef JOYSTICK_H
#define JOYSTICK_H
#include "Device.h"
#include <string>

#define JS_EVENT_AXIS 2

class Joystick : public Device {
 public:
  Joystick(std::string const & device_name);
  ~Joystick() {}
  int read_device_event(device_event& event) const;
};

#endif
