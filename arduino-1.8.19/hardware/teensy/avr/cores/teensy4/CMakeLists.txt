cmake_minimum_required(VERSION 3.10)

set(ARDUINOPATH "$ENV{HOME}/Embedded-A/arduino-1.8.19")
set(LIBRARYPATH "${ARDUINOPATH}/libraries")
set(COMPILERPATH "${ARDUINOPATH}/hardware/tools/arm/bin")

set(TEENSYLOADERPATH "$ENV{HOME}/pkgs/teensy_loader_cli-2.2")
set(TEENSYLIBPATH "${ARDUINOPATH}/hardware/teensy/avr/cores/teensy4")
set(AVRLIBRARYPATHS "${ARDUINOPATH}/hardware/teensy/avr/libraries")
set(CMAKE_OBJCOPY "${COMPILERPATH}/arm-none-eabi-objcopy")

add_subdirectory(${TEENSYLIBPATH}/bin)

file(GLOB SRC_FILES
    "src/*.cpp"
    "src/*.c"
    "${AVRLIBRARYPATHS}/Encoder/*.cpp"
    "${AVRLIBRARYPATHS}/TimerOne/*.cpp"
)

add_executable(image.elf ${SRC_FILES})

target_include_directories(image.elf PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    $ENV{HOME}/catkin_ws/src/2024-A/apogee_robot_core/include/
    ${AVRLIBRARYPATHS}/Encoder/
    ${AVRLIBRARYPATHS}/Encoder/utility/
    ${AVRLIBRARYPATHS}/TimerOne/
    ${AVRLIBRARYPATHS}/TimerThree/
    #${AVRLIBRARYPATHS}/hardware/teensy/avr/cores/teensy4/WProgram
)

target_link_libraries(image.elf teensy_lib)


add_custom_command( 
    TARGET image.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex -R .eeprom -R .fuse -R .lock -R .signature image.elf image.hex
    COMMAND ${TEENSYLOADERPATH}/teensy_loader_cli --mcu=${MCU} -w -v image.hex
    DEPENDS image.elf
)
