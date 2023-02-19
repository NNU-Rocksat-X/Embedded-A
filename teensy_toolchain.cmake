set(CMAKE_SYSTEM_PROCESSOR arm)

set(ARDUINOPATH "/home/daedalus2/pkgs/arduino-1.8.19")
set(COMPILERPATH "${ARDUINOPATH}/hardware/tools/arm/bin")
set(TEENSYLIBPATH "${ARDUINOPATH}/hardware/teensy/avr/cores/teensy4")

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_C_COMPILER "${COMPILERPATH}/arm-none-eabi-gcc")
set(CMAKE_CXX_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER "${COMPILERPATH}/arm-none-eabi-g++")
set(CMAKE_LINKER "${COMPILERPATH}/arm-none-eabi-ld")

set(CMAKE_CXX_STANDARD 11)
set(MCU "IMXRT1062")
set(MCU_LD "${TEENSYLIBPATH}/imxrt1062_t41.ld")
set(MCU_DEF "ARDUINO_TEENSY41")

set(OPTIONS "-DF_CPU=600000000 -DUSB_SERIAL -DLAYOUT_US_ENGLISH -DUSING_MAKEFILE -D__${MCU}__ -DARDUINO=10813 -DTEENSYDUINO=154 -D${MCU_DEF}")

set(CPUOPTIONS "-mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-d16 -mthumb")

set(CMAKE_C_FLAGS "-Wall -g -O2 ${CPUOPTIONS} -MMD ${OPTIONS} -I. -ffunction-sections -fdata-sections")
set(CMAKE_CXX_FLAGS "-Wall -g -O2 ${CPUOPTIONS} -MMD ${OPTIONS} -I. -ffunction-sections -fdata-sections -std=gnu++14 -felide-constructors -fno-exceptions -fpermissive -fno-rtti -Wno-error=narrowing")
set(CMAKE_EXE_LINKER_FLAGS "-Os -Wl,--gc-sections,--relax -T${MCU_LD} ${CPUOPTIONS}")
