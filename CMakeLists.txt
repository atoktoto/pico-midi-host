cmake_minimum_required(VERSION 3.12)

# Import Pico SDK
include(pico_sdk_import.cmake)
include(FreeRTOS_Kernel_import.cmake)

project(pico_examples C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_PLATFORM "rp2040")
set(PICO_DEOPTIMIZED_DEBUG 1)

# Initialize the SDK
pico_sdk_init()

set(FAMILY rp2040)
set(BOARD pico_sdk)

add_library(freertos STATIC
        ${FREERTOS_KERNEL_PATH}
        ${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/port.c
        )

target_include_directories(freertos PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/
        ${FREERTOS_KERNEL_PATH}/include
        ${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/include
        ${FREERTOS_KERNEL_PATH}/portable/GCC/ARM_CM0
        )

target_link_libraries(freertos
        FreeRTOS-Kernel
        FreeRTOS-Kernel-Heap3
        pico_stdlib
        )

add_subdirectory(lib/arduino_midi_library)
target_include_directories(midi
        PUBLIC lib/arduino_midi_library/src) # arduino_midi_library uses include_directories (so they are not exported)

add_subdirectory(lib/pico-midi-usb-transport)

add_executable(main)
pico_enable_stdio_uart(main 1)

target_sources(main PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/usb_descriptors.c
        )

target_include_directories(main PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        )

target_link_libraries(main
        pico_stdlib
        tinyusb_host
        tinyusb_board
        freertos
        midi
        pico-midi-usb
        )

# Create map/bin/hex file etc.
pico_add_extra_outputs(main)

