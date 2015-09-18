cmake_minimum_required (VERSION 2.6)
project (Steering)
add_executable(SteeringCommandDriver Joystick.cpp SteeringCommandPublisher.cpp SteeringCommandDriver.cpp)
target_link_libraries(SteeringCommandDriver lcm)
