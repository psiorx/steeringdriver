# steeringdriver
LCM Driver for the Driving Force GT steering wheel and for keyboard

## Operation
Follow these steps to compile and use the LCM Driver in Drake
### Compile the Source
The binary of the LCM Driver can be compiled from source as follows:

1. Clone the repository from github which contains the driver code  
`git clone https://github.com/psiorx/steeringdriver`
2. Navigate into the directory of the cloned repository  
`cd steeringdriver`
3. Create a build directory and navigate into it  
`mkdir build; cd build`
4. Run CMake to generate the build files  
`cmake ../`
5. Run make to generate the driver binary  
`make`

### Add the LCM type to Drake
Place a copy of the LCM type into your Drake distribution:

1. Navigate into the cloned repository  
`cd [PATH_TO_steeringdriver]`
2. Copy the LCM type to your Drake distribution  
`cp lcmtypes/lcmt_driving_control_cmd_t.lcm [PATH_TO_DRAKE_DISTRO]/drake/lcmtypes`
3. Navigate to your Drake distribution  
`cd [PATH_TO_DRAKE_DISTRO]`
4. Rebuild your Drake distribution  
`make clean; make`

### Run the LCM Driver
From the `build/` directory in your `steeringdriver` repo:

- To run the driver for use with a joystick, run:  
`./SteeringCommandDriver [PathToJoystick] [NameOfLCMTag]`  
Where `PathToJoystick` is the unix file for the joystick interface
(`/dev/input/js1` for my development computer), and `NameOfLCMTag` is
the name of the tag that the subscriber will listen for.
- To run the driver for use with a keyboard, run:  
`sudo ./SteeringCommandDriver [PathToKeyboard] [NameOfLCMTag]`  
Where `PathToKeyboard` is the unix file for the keyboard event interface
(`/dev/input/eventX` where `X` is the event number that corresponds to keyboard events),
and `NameOfLCMTag` corresponds to the tag that the subscriber is configured to listen to.
Be sure to use `sudo` when using the keyboard driver, as the binary requires sudo privileges to listen to keyboard events.
