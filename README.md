# Raspberry Pi Pico RP2040 MIDI Host example

This is an example project that uses multiple libraries to achieve MIDI host functionality on RP2040 microcontroller.

It uses:
- [Forked version of TinyUSB](https://github.com/atoktoto/tinyusb/tree/midihost) ([pull request](https://github.com/hathach/tinyusb/pull/1627) pending)
- [Arduino MIDI Library](https://github.com/FortySevenEffects/arduino_midi_library)
- A [Pico MIDI USB transport](https://github.com/atoktoto/pico-midi-usb-transport) for Arduino MIDI Library

The example also depends on FreeRTOS.
