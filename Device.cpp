#include "Device.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;

Device::Device(string const & device_name)
    : m_device_name(device_name), m_device_fd(-1) {
  open_device();
}

Device::~Device() {
  close_device();
}

void Device::open_device() {
  m_device_fd = open(m_device_name.c_str(), O_RDONLY | O_NONBLOCK);
}

void Device::close_device() {
  if (is_open()) {
    close(m_device_fd);
  }
}

bool Device::is_open() const {
  return m_device_fd >= 0;
}

