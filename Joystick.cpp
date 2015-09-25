#include "Joystick.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;

Joystick::Joystick(string const & device_name) : Device(device_name) {}

int Joystick::read_device_event(device_event& event) const {
  int bytes;
  bytes = read(m_device_fd, &event, sizeof(event)); 

  if (bytes == -1) return 0;
  if (bytes == sizeof(event)) return DEVICE_SUCCESS;

  printf("Unexpected bytes from device:%d\n", bytes);
  return -1;  
}

