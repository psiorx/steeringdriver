#include "Keyboard.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;

Keyboard::Keyboard(string const & device_name) : Device(device_name) {};

int Keyboard::read_device_event(device_event& event) const {
  int bytes;
  input_event ine;
  bytes = read(m_device_fd, &ine, sizeof(ine));

  if (bytes == -1) return 0;
  if (bytes == sizeof(ine)) {
    event.time = (ine.time.tv_sec * 1000) + (ine.time.tv_usec / 1000);
    event.value = ine.value;
    event.type = ine.type;
    event.number = ine.code;
    return DEVICE_SUCCESS;
  }

  printf("Unexpected bytes from device:%d\n", bytes);
  return -1;  
}
