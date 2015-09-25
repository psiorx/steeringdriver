#ifndef DEVICE_H
#define DEVICE_H
#include <string>
#include <linux/input.h>

#define DEVICE_SUCCESS 1

struct device_event {
	unsigned int time;    /* event timestamp in milliseconds */
	short value;          /* value */
	unsigned char type;   /* event type */
	unsigned char number; /* axis/button number */
};

class Device {
 public:
  Device(std::string const & device_name);
  ~Device();
  bool is_open() const;
  virtual int read_device_event(device_event& event) const = 0;
 protected:
  int m_device_fd;
 private:
  void open_device();
  void close_device();
  std::string m_device_name;
};

#endif
