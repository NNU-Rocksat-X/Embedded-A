cmake_minimum_required(VERSION 3.10)

set(ARDUINOPATH "$ENV{HOME}/pkgs/arduino-1.8.19")
set(LIBRARYPATH "${ARDUINOPATH}/libraries")
set(COMPILERPATH "${ARDUINOPATH}/hardware/tools/arm/bin")

set(TEENSYLOADERPATH "$ENV{HOME}/pkgs/teensy_loader_cli-2.2")
set(TEENSYLIBPATH "${ARDUINOPATH}/hardware/teensy/avr/cores/teensy4")
set(CMAKE_OBJCOPY "${COMPILERPATH}/arm-none-eabi-objcopy")

add_subdirectory(${TEENSYLIBPATH} ${TEENSYLIBPATH}/bin)

file(GLOB SRC_FILES
    "src/*.cpp"
    "src/*.c"
)

add_executable(image.elf ${SRC_FILES})

target_include_directories(image.elf PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    $ENV{HOME}/catkin_ws/src/daedalus/apogee_robot_core/include/
)

target_link_libraries(image.elf teensy_lib)


add_custom_command( 
    TARGET image.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex -R .eeprom -R .fuse -R .lock -R .signature image.elf image.hex
    COMMAND ${TEENSYLOADERPATH}/teensy_loader_cli --mcu=${MCU} -w -v image.hex
    DEPENDS image.elf
)
