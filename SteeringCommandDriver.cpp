#include "SteeringCommandPublisher.h"

int main(int argc, char *argv[])
{
  if (argc < 2 || argc > 3) {
    printf("Usage: SteeringCommandDriver [<joystick-device>] <lcm-channel>\n e.g. SteeringCommandDriver /dev/input/js1 STEERING_COMMAND\n");
    exit(1);
  }

  SteeringCommandPublisher steering_publisher = (argc == 2) ? SteeringCommandPublisher(argv[1]) : SteeringCommandPublisher(argv[1], argv[2]);

  while (steering_publisher.good()) {
    steering_publisher.publish();
  }

  return 0;
}
