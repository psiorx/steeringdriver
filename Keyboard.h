#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <string>
#include <linux/input.h>
#include "Device.h"

class Keyboard : public Device {
 public:
  Keyboard();
  ~Keyboard() {}
  int read_device_event(device_event& event) const;
};

#endif
