import pygame, lcm, argparse, sys
from lcmt_driving_control_cmd_t import lcmt_driving_control_cmd_t as lcm_msg

class KeyboardEventProcessor:
  def __init__(self):
    pygame.event.set_allowed([pygame.KEYUP, pygame.KEYDOWN])

class JoystickEventProcessor:
  def __init__(self):
    pygame.event.set_allowed([pygame.JOYAXISMOTION])

class SteeringCommandPublisher:
  def __init__(self, input_method):
    print 'Initializing...'
    pygame.init()
    if input_method == 'keyboard':
      self.event_processor = KeyboardEventProcessor();
    else:
      self.event_processor = JoystickEventProcessor();
    self.last_msg = lcm_msg()
    self.lc = lcm.LCM()
    print 'Ready'

  def start(self):
    while True:
      event = pygame.event.wait()
      print event

if __name__ == '__main__':
  parser = argparse.ArgumentParser(description='Publishes steering commands over LCM')
  parser.add_argument('--input_method', choices=['joystick', 'keyboard'], default='keyboard',
      help='the input method to use for publishing LCM steering commands (default keyboard)')
  args = parser.parse_args()

  publisher = SteeringCommandPublisher(args.input_method)
  publisher.start()

