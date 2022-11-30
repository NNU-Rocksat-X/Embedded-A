# Embedded Robot Control

## Installing
The embedded software is designed to run on a Teensy 4.1. The software can be loaded onto the teensy with two methods:
### Teensyduino
[Teensyduino](https://www.pjrc.com/teensy/td_download.html) (Arduino + Teensy Loader)

### Teensy Loader from the command line

## Do not need
Download teensy_loader_cli
  - `sudo apt-get install libusb-dev`
  - `wget https://github.com/PaulStoffregen/teensy_loader_cli/archive/refs/tags/2.2.tar.gz`
  - `sudo tar xzvf 2.2.tar.gz`
  - `cd teensy_loader_cli-2.2`
  - `sudo make`
## Do not need
  
Install rules (Gives Permission to load through USB).
  - `sudo wget https://www.pjrc.com/teensy/00-teensy.rules`
  - `sudo cp 00-teensy.rules /etc/udev/rules.d/00-teensy.rules`
   
Install Compiler (gcc)
  - `sudo apt-get install gcc-avr binutils-avr avr-libc`
  
Install Arduino
  - For x86 architecture: `wget https://downloads.arduino.cc/arduino-1.8.19-linux64.tar.xz`
    - `sudo tar -xf arduino-1.8.19-linux64.tar.xz`
  - For aarch64 architecture (Jetson Nano): `wget https://downloads.arduino.cc/arduino-1.8.19-linuxaarch64.tar.xz`
    - `sudo tar -xf arduino-1.8.19-linuxaarch64.tar.xz`
  - `cd arduino-1.8.19
  - `sudo ./install.sh`
  - `./arduino-linux-setup.sh $USER
  
  
Install Teensyduino
  - For aarch64 (Jetson): `wget https://www.pjrc.com/teensy/td_157/TeensyduinoInstall.linuxaarch64`
  - `chmod 755 TeensyduinoInstall.linux64`
  - `./TeensyduinoInstall.linux64 --dir=arduino-1.8.19`
  - `cd arduino-1.8.15/hardware/teensy/avr/cores/teensy4`
  - Edit `Makefile` and comment the lines under "Use these lines for Teensy 4.0" and uncomment under "User these lines for Teensy 4.1"
  
   
Load Program
  - ./teensy_loader_cli --mcu=TEENSY41 -w <file.hex>
  - Press button or jetson script to put in reset mode.
