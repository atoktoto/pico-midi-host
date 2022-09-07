
/**
 * This test program is designed to send and receive MIDI data concurrently
 * It send G, G#, A, A#, B over and over again on the highest cable number
 * of the MIDI device. The notes correspond to the Mackie Control transport
 * button LEDs, so most keyboards and controllers will react to these note
 * messages from the host. The host will print on the serial port console
 * any incoming messages along with the cable number that sent them.
 *
 * If you define MIDIH_TEST_KORG_NANOKONTROL2 to 1 and connect a Korg
 * nanoKONTROL2 controller to the host, then the host will request a dump
 * of the current scene, and the nanoKONTROL2 will respond with a long
 * sysex message (used to prove that sysex decoding appears to work)
 */

#include "midi_app.h"

static uint8_t midi_dev_addr = 0;


static void poll_midi_host_rx(void) {
    // device must be attached and have at least one endpoint ready to receive a message
    if (!midi_dev_addr || !tuh_midi_configured(midi_dev_addr)) {
        return;
    }
    if (tuh_midih_get_num_rx_cables(midi_dev_addr) < 1) {
        return;
    }

    tuh_midi_read_poll(midi_dev_addr); // if there is data, then the callback will be called
}


void midi_host_app_task(void) {
    if (cloning_is_required()) {
        if (midi_dev_addr != 0) {
            TU_LOG1("start descriptor cloning\r\n");
            clone_descriptors(midi_dev_addr);
        }
    } else if (clone_next_string_is_required()) {
        TU_LOG1("Cloning next string\r\n");
        clone_next_string();
    } else if (descriptors_are_cloned()) {
        poll_midi_host_rx();
        tuh_midi_stream_flush(midi_dev_addr);
    }
}

//--------------------------------------------------------------------+
// TinyUSB Callbacks
//--------------------------------------------------------------------+

// Invoked when device with hid interface is mounted
// Report descriptor is also available for use. tuh_hid_parse_report_descriptor()
// can be used to parse common/simple enough descriptor.
// Note: if report descriptor length > CFG_TUH_ENUMERATION_BUFSIZE, it will be skipped
// therefore report_desc = NULL, desc_len = 0
void tuh_midi_mount_cb(uint8_t dev_addr, uint8_t in_ep, uint8_t out_ep, uint8_t num_cables_rx, uint16_t num_cables_tx) {
    TU_LOG1("MIDI device address = %u, IN endpoint %u has %u cables, OUT endpoint %u has %u cables\r\n",
            dev_addr, in_ep & 0xf, num_cables_rx, out_ep & 0xf, num_cables_tx);

    midi_dev_addr = dev_addr;
    set_cloning_required();
}

// Invoked when device with hid interface is un-mounted
void tuh_midi_umount_cb(uint8_t dev_addr, uint8_t instance) {
    midi_dev_addr = 0;
    set_descriptors_uncloned();
    TU_LOG1("MIDI device address = %d, instance = %d is unmounted\r\n", dev_addr, instance);
}

void tuh_midi_rx_cb(uint8_t dev_addr, uint32_t num_packets) {
    if (midi_dev_addr != dev_addr) return;

    while (num_packets > 0) {
        --num_packets;
        uint8_t packet[4];
        if (tuh_midi_packet_read(dev_addr, packet)) {
            TU_LOG1("Read bytes %u %u %u %u\r\n", packet[0], packet[1], packet[2], packet[3]);
        }
    }
}

void tuh_midi_tx_cb(uint8_t dev_addr) {
    (void) dev_addr;
}
