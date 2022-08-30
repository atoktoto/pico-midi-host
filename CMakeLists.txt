cmake_minimum_required(VERSION 3.12)

# Import Pico SDK
include(pico_sdk_import.cmake)

project(pico_examples C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})

# Initialize the SDK
pico_sdk_init()

set(FAMILY rp2040)
set(BOARD pico_sdk)

add_executable(main)

target_sources(main PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/midi_app.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
        )

target_include_directories(main PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        )

 # Import pico_stdlib which imports commonly used features
 target_link_libraries(main PUBLIC pico_stdlib tinyusb_host tinyusb_board)

 # Create map/bin/hex file etc.
 pico_add_extra_outputs(main)