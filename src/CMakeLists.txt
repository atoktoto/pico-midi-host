add_executable(main)

target_sources(main PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/midi_app.c
        ${CMAKE_CURRENT_SOURCE_DIR}/main.c
        )

target_include_directories(main PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        )

 # Import pico_stdlib which imports commonly used features
 target_link_libraries(main PUBLIC pico_stdlib tinyusb_host tinyusb_board)

 # Create map/bin/hex file etc.
 pico_add_extra_outputs(main)

 # Add path via pico_set_program_url CMAKE
 example_auto_set_url(main)