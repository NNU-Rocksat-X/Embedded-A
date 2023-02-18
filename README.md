# Embedded Robot Control

## Installing
The embedded software is designed to run on a Teensy 4.1. While this repo can be placed anywhere, it depends on daedalus being installed in catkin_ws.
(Specifically Apogee_Robot_Core).

Note: This dependancy is because the packet structs are located in a shared
location so you do not have to worry about editing both sets of software. 
That shared location is currently in apogee_robot_core, however that shared location could be moved to this repo.

### Teensy Loader from the command line
Download teensy_loader_cli
  - `sudo apt-get install libusb-dev`
  - `wget https://github.com/PaulStoffregen/teensy_loader_cli/archive/refs/tags/2.2.tar.gz`
  - `sudo tar xzvf 2.2.tar.gz`
  - `cd teensy_loader_cli-2.2`
  - `sudo make`
  
Install rules (Gives Permission to load through USB).
  - `sudo wget https://www.pjrc.com/teensy/00-teensy.rules`
  - `sudo cp 00-teensy.rules /etc/udev/rules.d/00-teensy.rules`
   
Install Compiler (gcc)
  - `sudo apt-get install gcc-avr binutils-avr avr-libc`
  
### Arduino
Install Arduino
  - For x86 architecture: `wget https://downloads.arduino.cc/arduino-1.8.19-linux64.tar.xz`
    - `sudo tar -xf arduino-1.8.19-linux64.tar.xz`
  - For aarch64 architecture (Jetson Nano): `wget https://downloads.arduino.cc/arduino-1.8.19-linuxaarch64.tar.xz`
    - `sudo tar -xf arduino-1.8.19-linuxaarch64.tar.xz`
  - `cd arduino-1.8.19`
  - `sudo ./install.sh`
  - `./arduino-linux-setup.sh $USER`
  
### Teensyduino
Install Teensyduino
  - For aarch64 (Jetson): `wget https://www.pjrc.com/teensy/td_157/TeensyduinoInstall.linuxaarch64`
  - `chmod 755 TeensyduinoInstall.linux64`
  - `./TeensyduinoInstall.linux64 --dir=arduino-1.8.19`
  - `cd arduino-1.8.15/hardware/teensy/avr/cores/teensy4`
  - Edit `Makefile` and comment the lines under "Use these lines for Teensy 4.0" and uncomment under "User these lines for Teensy 4.1"
  
## Building
Edit `teensy_toolchain.cmake`
Replace `/home/jet` with your username
Move `Teensy4_CmakeLists.txt` to `arduino-1.8.19/hardware/teensy/avr/cores/teensy4/
Rename the Teensy4_CMakeLists.txt you move to CMakeLists.txt

Then Create the build directory
`cmake -D CMAKE_TOOLCHAIN_FILE=./teensy_toolchain.cmake -B./build -H./`

Then anytime you wish to build and load software onto the teensy:

From ./build: `make`

## Teensy Danger Warning

Building automatically attempts to load software onto the teensy. However, this is only possible through the micro-usb port on the teensy. However, the teensy will be powered through its 5V pin from the PCB. Providing power from both the Micro USB cable and the PCB will break the teensy.

**DO NOT POWER THE TEENSY FROM MICRO-USB AND PCB SIMULTANEOUSLY**

Here are your options:
- https://www.pjrc.com/teensy/external_power.html
- Leave the micro-usb cable in the teensy, but unplug the usb cable from the jetson every time before PCB power is activated
- Do anything, but do not power the teensy from the jetson while PCB power is on

## TODO
Need to write software that triggers Jetson Pin high which is connected to program pin on teensy. This allows for programming remotely, otherwise someone has to press the button on the teensy to program.
