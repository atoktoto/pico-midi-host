
#include <sys/cdefs.h>
#include <pico/platform.h>
#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"

#include "FreeRTOS.h"
#include "task.h"

#include "bsp/board.h"
#include "tusb.h"
#include "class/midi/midi_host.h"

#include "MIDI.h"
#include "TUSB-MIDI.hpp"

#define USBD_STACK_SIZE    (3*configMINIMAL_STACK_SIZE/2) * (CFG_TUSB_DEBUG ? 2 : 1)

_Noreturn void led_blinking_task(void *pvParams);
_Noreturn void midi_host_task(void *pvParams);
_Noreturn void midi_host_tx_task(void *pvParams);

using namespace usbMidi;
using namespace midi;

UsbMidiTransport usbMIDITransport(0);
MidiInterface<UsbMidiTransport> MIDI((UsbMidiTransport&)usbMIDITransport);

void onNote(Channel channel, byte note, byte velocity) {
    printf("Note ON ch=%d, note=%d, vel=%d\n", channel, note, velocity);
}

void offNote(Channel channel, byte note, byte velocity) {
    printf("Note OFF ch=%d, note=%d, vel=%d\n", channel, note, velocity);
}

void onControlChange(Channel channel, byte b1, byte b2) {
    printf("ControlChange ch=%d, b1=%d, b2=%d\n", channel, b1, b2);
}

int main() {

    stdio_init_all();
    board_init();

    printf("TinyUSB Host MIDI Example\n");

    xTaskCreate(led_blinking_task, "blink", 256, nullptr, tskIDLE_PRIORITY, nullptr);
    xTaskCreate(midi_host_task, "tuh", USBD_STACK_SIZE, nullptr, configMAX_PRIORITIES - 1, nullptr);
    xTaskCreate(midi_host_tx_task, "tuh-tx", USBD_STACK_SIZE, nullptr, tskIDLE_PRIORITY, nullptr);

    MIDI.begin(11);
    MIDI.setHandleNoteOn(onNote);
    MIDI.setHandleNoteOff(offNote);
    MIDI.setHandleControlChange(onControlChange);

    vTaskStartScheduler();
    return 0;
}

_Noreturn void midi_host_tx_task(void *pvParams) {

    uint8_t i = 0;

    while(true) {
        vTaskDelay(pdMS_TO_TICKS(20));
        MIDI.sendNoteOn(i + 8, 127, 11); //or 11
        vTaskDelay(pdMS_TO_TICKS(50));
        MIDI.sendNoteOff(i + 8, 0, 11); //or 11

        i++;
        i = i % 16;
    }
}

_Noreturn void midi_host_task(void *pvParams) {
    tusb_init();
    while (true) {
        tuh_task();
        MIDI.read();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

_Noreturn void led_blinking_task(void *pvParams) {
    static bool led_state = false;
    while (true) {
        board_led_write(led_state);
        led_state = !led_state;
        vTaskDelay(200);
    }
}

//--------------------------------------------------------------------+
// TinyUSB Callbacks
//--------------------------------------------------------------------+

// Invoked when device with hid interface is mounted
void tuh_midi_mount_cb(uint8_t dev_addr, uint8_t in_ep, uint8_t out_ep, uint8_t num_cables_rx, uint16_t num_cables_tx) {
    printf("MIDI device address = %u, IN endpoint %u has %u cables, OUT endpoint %u has %u cables\r\n",
            dev_addr, in_ep & 0xf, num_cables_rx, out_ep & 0xf, num_cables_tx);
    usbMIDITransport.midi_dev_addr = dev_addr;
}

// Invoked when device with hid interface is un-mounted
void tuh_midi_umount_cb(uint8_t dev_addr, uint8_t instance) {
    printf("MIDI device address = %d, instance = %d is unmounted\r\n", dev_addr, instance);
    usbMIDITransport.midi_dev_addr = 0;
}

void tuh_midi_rx_cb(uint8_t dev_addr, uint32_t num_packets) {
    usbMIDITransport.tuh_midi_rx_cb(dev_addr, num_packets);
}

void tuh_midi_tx_cb(uint8_t dev_addr) {
    (void) dev_addr;
}

//--------------------------------------------------------------------+
// freeRTOS hooks
//--------------------------------------------------------------------+

extern "C" {
void vApplicationMallocFailedHook(void) {
    TU_LOG1("Malloc failed!\r\n");
    configASSERT((volatile void *) NULL); // Force an assert.
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    TU_LOG1("Stack overflow!\r\n");
    configASSERT((volatile void *) NULL); // Force an assert.
}
}
